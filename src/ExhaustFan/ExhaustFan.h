#ifndef ExhaustFan_h
#define ExhaustFan_h

#include "../DigitalOutput/DigitalOutput.h"
#include "Arduino.h"

class ExhaustFan : public DigitalOutput {
   public:
    ExhaustFan(int pin);
};

#endif