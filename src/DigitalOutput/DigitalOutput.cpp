#include "DigitalOutput.h"

DigitalOutput::DigitalOutput(int pin) : _pin(pin) {}

void DigitalOutput::begin() {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
}

void DigitalOutput::on() {
    digitalWrite(_pin, HIGH);
    _isOn = true;
}

void DigitalOutput::off() {
    digitalWrite(_pin, LOW);
    _isOn = false;
}

void DigitalOutput::toggle() {
    if (_isOn) {
        off();
    } else {
        on();
    }
}

void DigitalOutput::onIf(bool condition) {
    // Serial.println(_pin);
    if (condition) {
        on();
    } else {
        off();
    }
}

bool DigitalOutput::isOn() {
    return _isOn;
}

int DigitalOutput::getPin() {
    return _pin;
}