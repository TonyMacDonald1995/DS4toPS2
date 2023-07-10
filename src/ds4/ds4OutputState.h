#include "pico/stdlib.h"

class DS4OutputState {
public:
    DS4OutputState();
    uint8_t rumbleLeft;
    uint8_t rumbleRight;

    uint8_t ledR;
    uint8_t ledG;
    uint8_t ledB;

    uint8_t flashOn;
    uint8_t flashOff;
};