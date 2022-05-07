#ifndef Button_h
#define Button_h

#include "../DIgitalInput/DigitalInput.h"
#include "Arduino.h"

class Button : public DigitalInput {
   public:
    Button(int pin);
    bool isPressed();
};

#endif