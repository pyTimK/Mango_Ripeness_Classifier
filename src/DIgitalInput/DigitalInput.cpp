#include "DigitalInput.h"

DigitalInput::DigitalInput(int pin) : _pin(pin) {}

void DigitalInput::begin() {
    pinMode(_pin, INPUT);
}

bool DigitalInput::isHigh() {
    return digitalRead(_pin);
}