#ifndef EthyleneSensor_h
#define EthyleneSensor_h

#include <SoftwareSerial.h>
// #include <WinsenZE03.h>

#include "Arduino.h"

class EthyleneSensor {
   public:
    enum MeasurementType { Analog,
                           Digital };

    EthyleneSensor(int rx_pin, int tx_pin, int analog_voltage_pin, int ppm_check_interval, bool is_digital);

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
    // WinsenZE03 _ethyleneSensor;
    unsigned long _lastMeasuredPPM = millis();
    int _ppm_check_interval;
    EthyleneSensor::MeasurementType _measurementType;
    bool _needToCheckTemp();

    void _initializeQnAMode();
    unsigned char _checkSum(unsigned char *i, unsigned char ln);
    void _printBuffer(byte *buffer, int length);
};

#endif