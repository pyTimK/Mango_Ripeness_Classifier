#include "Button.h"

Button::Button(int pin) : DigitalInput(pin) {}

bool Button::isPressed() {
    return isHigh();
}