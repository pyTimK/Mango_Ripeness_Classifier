#ifndef DigitalOutput_h
#define DigitalOutput_h

#include "Arduino.h"

class DigitalOutput {
   public:
    DigitalOutput(int pin);
    void begin();
    void on();
    void off();
    void toggle();
    void onIf(bool condition);
    bool isOn();
    int getPin();

   private:
    int _pin;
    bool _isOn;
};

#endif