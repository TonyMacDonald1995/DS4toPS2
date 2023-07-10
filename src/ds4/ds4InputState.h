#include "pico/stdlib.h"

class DS4InputState {
public:
    DS4InputState();
    uint8_t buttons1;
    uint8_t buttons2;
    uint8_t lx;
    uint8_t ly;
    uint8_t rx;
    uint8_t ry;
    uint8_t l2;
    uint8_t r2;
    bool psButton;
};