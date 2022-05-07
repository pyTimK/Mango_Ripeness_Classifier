#include "IRSensor.h"

IRSensor::IRSensor(int pin) : DigitalInput(pin) {}

bool IRSensor::objectDetected() {
    return !isHigh();
}