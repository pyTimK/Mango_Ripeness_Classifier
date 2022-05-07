#include "StatusManager.h"

#include "../LCD/LCD.h"
#include "../TempSensor/TempSensor.h"

StatusManager::StatusManager(int measuring_sec, int showing_error_sec) : _measuring_sec(measuring_sec), _showing_error_sec(showing_error_sec) {}

void StatusManager::begin(TempSensor *_tempSensor, LCD *lcd) {
    _tempSensor = tempSensor;
    _lcd = lcd;
}

void StatusManager::setStatus(Status newStatus) {
    status = newStatus;
    _lcd->clear();
}

void StatusManager::update(bool mangoDetected, bool buttonPressed) {
    switch (status) {
        case CALIBRATING:
            _tempSensor->measure();
            if (_tempSensor->withinThreshold()) {
                setStatus(IDLE);
            }
            break;

        case IDLE:
            _tempSensor->intervalMeasure();

            if (!_tempSensor->withinThreshold()) {
                setStatus(CALIBRATING);
            } else {
                if (buttonPressed) {
                    if (mangoDetected) {
                        setStatus(MEASURING);
                        _startedMeasuring = millis();

                    } else {
                        setStatus(SHOWING_ERROR);
                        errorCode = NO_MANGO;
                        _startedShowing = millis();
                    }
                }
            }

            break;

        case SHOWING_ERROR:
            if (errorCode == NO_MANGO && mangoDetected) {
                setStatus(IDLE);
                break;
            }

            if (_isDoneShowingError()) {
                setStatus(IDLE);
            }
            break;

        case MEASURING:
            if (_isDoneMeasuring()) {
                setStatus(SHOWING_RESULT);
                _startedShowing = millis();
                break;
            }
            break;

        case SHOWING_RESULT:
            if (!mangoDetected) {
                setStatus(IDLE);
            }
            break;
    }
}

bool StatusManager::_isDoneMeasuring() {
    if (status != MEASURING) return true;
    return millis() - _startedMeasuring > _measuring_sec * 1000;
}

bool StatusManager::_isDoneShowingError() {
    if (status != SHOWING_ERROR) return true;
    return millis() - _startedShowing > _showing_error_sec * 1000;
}

int StatusManager::getMeasuringTimeLeft() {
    if (status != MEASURING) return 0;
    return _measuring_sec - (millis() - _startedMeasuring) / 1000;
}