#ifndef MyLed_h
#define MyLed_h

#include "../DigitalOutput/DigitalOutput.h"
#include "Arduino.h"
class MyLed : public DigitalOutput {
   public:
    MyLed(int pin);
};

#endif