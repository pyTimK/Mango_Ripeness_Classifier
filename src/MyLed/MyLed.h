#ifndef MyLed_h
#define MyLed_h

#include "Arduino.h"

class MyLed {
   public:
    MyLed(int pin);
    void begin();
    void on();
    void off();
    void toggle();
    void onIf(bool condition);
    bool isOn();

   private:
    int _pin;
    bool _isOn;
};

#endif