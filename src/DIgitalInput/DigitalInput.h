#ifndef DigitalInput_h
#define DigitalInput_h

#include "Arduino.h"

class DigitalInput {
   public:
    DigitalInput(int pin);
    void begin();

   protected:
    bool isHigh();

   private:
    int _pin;
};

#endif