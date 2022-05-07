#include "EthyleneSensor.h"

EthyleneSensor::EthyleneSensor(int rx_pin, int tx_pin, int analog_voltage_pin, int ppm_check_interval) : _analog_voltage_pin(analog_voltage_pin), _ethyleneSerial(rx_pin, tx_pin), _ethyleneSensor(), _ppm_check_interval(ppm_check_interval) {}

void EthyleneSensor::begin() {
    _ethyleneSerial.begin(9600);
    _ethyleneSensor.begin(&_ethyleneSerial, 2);
    _ethyleneSensor.setAs(QA);
}

float EthyleneSensor::getDigitalPPM() {
    return _ethyleneSensor.readManual();
}

float EthyleneSensor::getAnalogPPM() {
    const int rawVoltage = analogRead(_analog_voltage_pin);
    const float voltage = rawVoltage * (5.0 / 1023.0);
    return voltage * _calibration;
}

float EthyleneSensor::measure() {
    value = _measurementType == EthyleneSensor::MeasurementType::Analog ? getAnalogPPM() : getDigitalPPM();
    _lastMeasuredPPM = millis();
    return value;
}

void EthyleneSensor::intervalMeasure() {
    if (_needToCheckTemp()) {
        measure();
    }
}

bool EthyleneSensor::_needToCheckTemp() {
    return millis() - _lastMeasuredPPM > _ppm_check_interval * 1000;
}