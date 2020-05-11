#pragma once

#include "Arduino.h"

void chase(uint32_t c);

enum class Leds {
    Blue1,
    Blue2,
    Blue3,
    Red1,
    Red2,
    Red3,
    Toggles
};

struct ButtonArray {
    bool blue1;
    bool blue2;
    bool blue3;
    bool red1;
    bool red2;
    bool red3;
    bool up;
    bool down;
    bool left;
    bool right;
}

void SwitchLed(Leds led, bool enabled);
void HandleButtons();