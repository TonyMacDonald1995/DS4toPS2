#include <string>
#include "pico/multicore.h"
#include "btstack_config.h"
#include "btstack.h"
#include "ds4InputState.h"
#include "ds4OutputState.h"

#define MAX_ATTRIBUTE_VALUE_SIZE 512

struct __attribute__((packed)) inputReport17 {
    uint8_t header[2];
    uint8_t reportId;
    uint8_t lx, ly;
    uint8_t rx, ry;
    uint8_t buttons[3];
    uint8_t l2, r2;
    uint16_t timestamp;
    uint8_t batteryTemp;
    uint16_t gyro[3];
    uint16_t accel[3];
    uint8_t unk[5];
    uint8_t status;

};

class DS4 {
public:
    DS4();
    DS4InputState getInputState();
    static bool connected;
    void setOutputState(DS4OutputState outputState);
    void init(std::string remoteAddrString);
private:
    static DS4InputState inputState;
    static DS4OutputState outputState;
    std::string remoteAddrString;
    static bd_addr_t remoteAddr;
    static bd_addr_t connectedAddr;
    btstack_packet_callback_registration_t hciEventCallbackRegistration;
    uint8_t hidDescriptorStorage[MAX_ATTRIBUTE_VALUE_SIZE];
    static uint16_t hidHostCid;
    static bool hidHostDescriptorAvailable;
    static void packetHandler(uint8_t packetType, uint16_t channel, uint8_t* packet, uint16_t size);
    static void processHidEvent(uint8_t* packet);
    void hidHostSetup();
    static void hidHostHandleInterruptReport(const uint8_t* packet, uint16_t packet_len);
    static void btHidDisconnected(bd_addr_t addr);
    static void sendCmd();
};