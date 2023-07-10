#include "ds4OutputState.h"

DS4OutputState::DS4OutputState() {
    this->rumbleLeft = 0;
    this->rumbleRight = 0;

    this->ledR = 0;
    this->ledG = 0;
    this->ledB = 0x80;
    
    this->flashOn = 0xff;
    this->flashOff = 0x00;
}