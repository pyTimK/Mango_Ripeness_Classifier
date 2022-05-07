#include "EthyleneSensor.h"

EthyleneSensor::EthyleneSensor(int rx_pin, int tx_pin, int analog_voltage_pin, int ppm_check_interval, bool is_digital)
    : _analog_voltage_pin(analog_voltage_pin),
      _ethyleneSerial(rx_pin, tx_pin),
      _ppm_check_interval(ppm_check_interval),
      _measurementType(is_digital ? EthyleneSensor::MeasurementType::Digital : EthyleneSensor::MeasurementType::Analog) {}

void EthyleneSensor::begin() {
    _ethyleneSerial.begin(9600);
    _initializeQnAMode();
    // _ethyleneSensor.begin(&_ethyleneSerial, 2);
    // _ethyleneSensor.setAs(QA);
}

float EthyleneSensor::getDigitalPPM() {
    const byte QnACommand[] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
    byte replyBuffer[9] = {};

    _ethyleneSerial.write(QnACommand, sizeof(QnACommand));
    _ethyleneSerial.readBytes(replyBuffer, sizeof(replyBuffer));

    const byte checkSum = _checkSum(replyBuffer, sizeof(replyBuffer));

    if (checkSum != replyBuffer[8]) {
        Serial.println("CheckSum error");
        return value;
    }

    // _printBuffer(replyBuffer, sizeof(replyBuffer));

    return (replyBuffer[2] * 256 + replyBuffer[3]) * 0.1;
}

void EthyleneSensor::_printBuffer(byte *buffer, int length) {
    for (int i = 0; i < length - 1; i++) {
        Serial.print(buffer[i], HEX);
        Serial.print(" ");
    }
    Serial.println(buffer[length - 1], HEX);
}

float EthyleneSensor::getAnalogPPM() {
    const int rawVoltage = analogRead(_analog_voltage_pin);
    const float voltage = rawVoltage * (5.0 / 1023.0);
    return voltage * _calibration;
}

float EthyleneSensor::measure() {
    value = _measurementType == EthyleneSensor::MeasurementType::Analog ? getAnalogPPM() : getDigitalPPM();
    _lastMeasuredPPM = millis();
    Serial.println(value);
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

void EthyleneSensor::_initializeQnAMode() {
    const byte switchToQnAcommand[] = {0xFF, 0x01, 0x78, 0x04, 0x00, 0x00, 0x00, 0x00, 0x83};
    byte expectedReply[9] = {0xFF, 0x78, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87};
    byte replyBuffer[9] = {};

    _ethyleneSerial.write(switchToQnAcommand, sizeof(switchToQnAcommand));
    _ethyleneSerial.readBytes(replyBuffer, sizeof(replyBuffer));

    for (int i = 0; i < 9; i++) {
        if (replyBuffer[i] != expectedReply[i]) {
            Serial.println("QnA mode cannot initialize");
            return;
        }
    }
    Serial.println("QnA mode initialized");
}

unsigned char EthyleneSensor::_checkSum(unsigned char *i, unsigned char ln) {
    unsigned char j, tempq = 0;
    i += 1;
    for (j = 0; j < (ln - 2); j++) {
        tempq += *i;
        i++;
    }
    tempq = (~tempq) + 1;
    return (tempq);
}