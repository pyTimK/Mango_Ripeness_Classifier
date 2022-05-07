#ifndef TempSensor_h
#define TempSensor_h

#include <DallasTemperature.h>
#include <OneWire.h>

#include "Arduino.h"

class TempSensor {
   public:
    TempSensor(int pin, float threshold, int temp_check_interval);
    void begin();
    float measure();
    bool withinThreshold();
    void intervalMeasure();
    float value;

   private:
    float _threshold;
    OneWire _oneWire;
    DallasTemperature _dallasTemperature;
    unsigned long _lastMeasuredTemp = millis();
    bool _needToCheckTemp();
    int _temp_check_interval;
};

#endif