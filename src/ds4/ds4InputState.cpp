#include "ds4InputState.h"

DS4InputState::DS4InputState() {
    this->buttons1 = 0xff;
    this->buttons2 = 0xff;
    this->lx = 0x80;
    this->ly = 0x80;
    this->rx = 0x80;
    this->ry = 0x80;
    this->l2 = 0x00;
    this->r2 = 0x00;
    this->psButton = false;
}