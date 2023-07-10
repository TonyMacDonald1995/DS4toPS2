#include "ds4.h"

#define MAX_ATTRIBUTE_VALUE_SIZE 512

uint16_t DS4::hidHostCid;
bool DS4::hidHostDescriptorAvailable;
bool DS4::connected;
bd_addr_t DS4::remoteAddr;
bd_addr_t DS4::connectedAddr;
DS4InputState DS4::inputState;
DS4OutputState DS4::outputState;

DS4::DS4() {

}

DS4InputState DS4::getInputState() {
    return inputState;
}

void DS4::setOutputState(DS4OutputState outputState) {
    this->outputState = outputState;
}

void DS4::init(std::string remoteAddrString) {
    this->remoteAddrString = remoteAddrString;
    this->hidHostCid = 0;
    this->hidHostDescriptorAvailable = false;
    this->connected = false;
    gap_set_security_level(LEVEL_2);
    hidHostSetup();
    sscanf_bd_addr(remoteAddrString.c_str(), remoteAddr);
    btHidDisconnected(remoteAddr);
    hci_power_control(HCI_POWER_ON);
}

void DS4::hidHostSetup() {
    l2cap_init();
    sdp_init();
    hid_host_init(hidDescriptorStorage, sizeof(hidDescriptorStorage));
    hid_host_register_packet_handler(packetHandler);
    gap_set_default_link_policy_settings(LM_LINK_POLICY_ENABLE_SNIFF_MODE | LM_LINK_POLICY_ENABLE_ROLE_SWITCH);
    hci_set_master_slave_policy(HCI_ROLE_MASTER);
    hciEventCallbackRegistration.callback = &packetHandler;
    hci_add_event_handler(&hciEventCallbackRegistration);
}

void DS4::packetHandler(uint8_t packetType, uint16_t channel, uint8_t* packet, uint16_t size) {

    uint8_t event;
    bd_addr_t eventAddr;

    if(packetType != HCI_EVENT_PACKET)
        return;

    event = hci_event_packet_get_type(packet);
    switch(event) {
        case BTSTACK_EVENT_STATE: {
            if(btstack_event_state_get_state(packet) == HCI_STATE_WORKING) {
                hid_host_connect(remoteAddr, HID_PROTOCOL_MODE_REPORT, &hidHostCid);
            }
            break;
        }
        case HCI_EVENT_PIN_CODE_REQUEST: {
            hci_event_pin_code_request_get_bd_addr(packet, eventAddr);
            gap_pin_code_response(eventAddr, "0000");
            break;
        }
        case HCI_EVENT_HID_META: {
            processHidEvent(packet);
            break;
        }
        case HCI_EVENT_DISCONNECTION_COMPLETE: {
            inputState = DS4InputState();
            connected = false;
            break;
            
        }
    }
}

void DS4::processHidEvent(uint8_t* packet) {

    uint8_t hidEvent = hci_event_hid_meta_get_subevent_code(packet);
    bd_addr_t eventAddr;
    switch(hidEvent) {
        case HID_SUBEVENT_INCOMING_CONNECTION: {
            hid_subevent_incoming_connection_get_address(packet, eventAddr);
            hid_host_accept_connection(hid_subevent_incoming_connection_get_hid_cid(packet), HID_PROTOCOL_MODE_REPORT);
            break;
        }
        case HID_SUBEVENT_CONNECTION_OPENED: {
            uint8_t status = hid_subevent_connection_opened_get_status(packet);
            hid_subevent_connection_opened_get_bd_addr(packet, eventAddr);
            if(status != ERROR_CODE_SUCCESS) {
                if(status == L2CAP_CONNECTION_RESPONSE_RESULT_REFUSED_SECURITY) {
                    gap_drop_link_key_for_bd_addr(eventAddr);
                    return;
                }
                btHidDisconnected(eventAddr);
                return;
            }
            hidHostDescriptorAvailable = false;
            hidHostCid = hid_subevent_connection_opened_get_hid_cid(packet);
            bd_addr_copy(connectedAddr, eventAddr);
            break;
        }
        case HID_SUBEVENT_DESCRIPTOR_AVAILABLE: {
            uint8_t status = hid_subevent_descriptor_available_get_status(packet);
            if(status == ERROR_CODE_SUCCESS) {
                hidHostDescriptorAvailable = true;
                connected = true;
                hid_host_send_get_report(hidHostCid, HID_REPORT_TYPE_FEATURE, 0x05);
            }
            break;
        }
        case HID_SUBEVENT_REPORT: {
            if(hidHostDescriptorAvailable) {
                hidHostHandleInterruptReport(hid_subevent_report_get_report(packet), hid_subevent_report_get_report_len(packet));
                sendCmd();
            }
            break;
        }
        case HID_SUBEVENT_CONNECTION_CLOSED: {
            btHidDisconnected(remoteAddr);
            break;
        }
    }

}

void DS4::btHidDisconnected(bd_addr_t addr) {
    hidHostCid = 0;
    hidHostDescriptorAvailable = false;
}

void DS4::hidHostHandleInterruptReport(const uint8_t* packet, uint16_t packetLen) {
    if(packetLen < sizeof(struct inputReport17) + 1)
        return;

    if(packet[0] != 0xa1 || packet[1] != 0x11)
        return;

    struct inputReport17 *report = (struct inputReport17*)&packet[1];

    uint8_t dpad = report->buttons[0] & 0b00001111;
	bool up = (dpad == 7) || (dpad == 0) || (dpad == 1);
	bool right = (dpad == 1) || (dpad == 2) || (dpad == 3);
	bool down = (dpad == 3) || (dpad == 4) || (dpad == 5);
	bool left = (dpad == 5) || (dpad == 6) || (dpad == 7);

    inputState.buttons1 = ~(((report->buttons[1] & 0x10 ? 1 : 0) << 0) | ((report->buttons[1] & 0x40 ? 1 : 0) << 1) | ((report->buttons[1] & 0x80 ? 1 : 0) << 2) | ((report->buttons[1] & 0x20 ? 1 : 0) << 3) |
		                    (up << 4) | (right << 5) | (down << 6) | (left << 7));
    inputState.buttons2 = ~(((report->buttons[1] & 0x04 ? 1 : 0) << 0) | ((report->buttons[1] & 0x08 ? 1 : 0) << 1) | ((report->buttons[1] & 0x01 ? 1 : 0) << 2) | ((report->buttons[1] & 0x02 ? 1 : 0) << 3) |
		              ((report->buttons[0] & 0x80 ? 1 : 0) << 4) | ((report->buttons[0] & 0x40 ? 1 : 0) << 5) | ((report->buttons[0] & 0x20 ? 1 : 0) << 6) | ((report->buttons[0] & 0x10 ? 1 : 0) << 7));

    inputState.lx = report->lx;
    inputState.ly = report->ly;
    inputState.rx = report->rx;
    inputState.ry = report->ry;
    inputState.l2 = report->l2;
    inputState.r2 = report->r2;
    inputState.psButton = report->buttons[2] & 0x01;
}

void DS4::sendCmd() {
    uint8_t buf[77] = {0xb0, 0x00, 0x0f};

    buf[5] = outputState.rumbleRight;
    buf[6] = outputState.rumbleLeft;

    buf[7] = outputState.ledR;
    buf[8] = outputState.ledG;
    buf[9] = outputState.ledB;

    buf[10] = outputState.flashOn;
    buf[11] = outputState.flashOff;

    hid_host_send_set_report(hidHostCid, HID_REPORT_TYPE_OUTPUT, 0x11, buf, sizeof(buf));
}