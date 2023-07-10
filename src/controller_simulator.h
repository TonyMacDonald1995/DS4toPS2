#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "pico/multicore.h"

#include "psxSPI.pio.h"
#include "ds4/ds4.h"

inline constexpr unsigned char operator "" _uc( unsigned long long arg ) noexcept
{
    return static_cast< unsigned char >( arg );
}

#define MODE_DIGITAL 0x41
#define MODE_ANALOG  0x73
#define MODE_ANALOG_PRESSURE 0x79
#define MODE_CONFIG 0xF3

#define CMD_PRES_CONFIG 0x40
#define CMD_POLL_CONFIG_STATUS 0x41
#define CMD_POLL 0x42
#define CMD_CONFIG 0x43
#define CMD_STATUS 0x45
#define CMD_CONST_46 0x46
#define CMD_CONST_47 0x47
#define CMD_CONST_4C 0x4C
#define CMD_ENABLE_RUMBLE 0x4D
#define CMD_POLL_CONFIG 0x4F
#define CMD_ANALOG_SWITCH 0x44

#define UP    0b00010000
#define RIGHT 0b00100000
#define DOWN  0b01000000
#define LEFT  0b10000000

#define L2    0b00000001
#define R2    0b00000010
#define L1    0b00000100
#define R1    0b00001000
#define TRI   0b00010000
#define CIR   0b00100000
#define X     0b01000000
#define SQU   0b10000000

void psx_main();
void processPoll();
void processConfig();
void processStatus();
void processConst46();
void processConst47();
void processConst4c();
void processPollConfigStatus();
void processEnableRumble();
void processPollConfig();
void processPresConfig();
void processAnalogSwitch();
void process_joy_req();