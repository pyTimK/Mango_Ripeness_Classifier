#include "MyLed.h"

MyLed::MyLed(int pin) {
    _pin = pin;
    _isOn = false;
}

void MyLed::begin() {
    pinMode(_pin, OUTPUT);
}

void MyLed::on() {
    digitalWrite(_pin, HIGH);
    _isOn = true;
}

void MyLed::off() {
    digitalWrite(_pin, LOW);
    _isOn = false;
}

void MyLed::toggle() {
    if (_isOn) {
        off();
    } else {
        on();
    }
}

void MyLed::onIf(bool condition) {
    if (condition) {
        on();
    } else {
        off();
    }
}

bool MyLed::isOn() {
    return _isOn;
}