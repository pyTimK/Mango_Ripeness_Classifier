#ifndef EthyleneSensor_h
#define EthyleneSensor_h

#include <SoftwareSerial.h>
#include <WinsenZE03.h>

#include "Arduino.h"

class EthyleneSensor {
   public:
    enum MeasurementType { Analog,
                           Digital };

    EthyleneSensor(int rx_pin, int tx_pin, int analog_voltage_pin, int ppm_check_interval);

    void begin();
    float getDigitalPPM();
    float getAnalogPPM();
    float value;
    float measure();
    void intervalMeasure();

   private:
    int _analog_voltage_pin;
    float _calibration = 46.9 / 1.366;
    SoftwareSerial _ethyleneSerial;
    WinsenZE03 _ethyleneSensor;
    unsigned long _lastMeasuredPPM = millis();
    int _ppm_check_interval;
    EthyleneSensor::MeasurementType _measurementType = EthyleneSensor::MeasurementType::Analog;
    bool _needToCheckTemp();
};

#endif