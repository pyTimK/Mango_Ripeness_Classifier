#include "TempSensor.h"

// OneWire oneWire(TEMP_PIN);
// DallasTemperature tempSensor(&oneWire);

TempSensor::TempSensor(int pin, float threshold, int temp_check_interval) : _threshold(threshold), _oneWire(pin), _dallasTemperature(&_oneWire), _temp_check_interval(temp_check_interval) {}

void TempSensor::begin() {
    _dallasTemperature.begin();
}

float TempSensor::measure() {
    _dallasTemperature.requestTemperatures();
    value = _dallasTemperature.getTempCByIndex(0);
    _lastMeasuredTemp = millis();
    return value;
}

bool TempSensor::withinThreshold() {
    return value <= _threshold;
}

void TempSensor::intervalMeasure() {
    if (_needToCheckTemp()) {
        measure();
    }
}

bool TempSensor::_needToCheckTemp() {
    return millis() - _lastMeasuredTemp > _temp_check_interval * 1000;
}