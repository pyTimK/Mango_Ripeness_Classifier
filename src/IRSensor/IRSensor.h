#ifndef IRSensor_h
#define IRSensor_h

#include "../DigitalInput/DigitalInput.h"
#include "Arduino.h"

class IRSensor : public DigitalInput {
   public:
    IRSensor(int pin);
    bool objectDetected();
};

#endif