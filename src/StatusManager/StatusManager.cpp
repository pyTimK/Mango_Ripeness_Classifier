#include "StatusManager.h"

StatusManager::StatusManager(int measuring_sec, int showing_error_sec) : _measuring_sec(measuring_sec), _showing_error_sec(showing_error_sec) {}

Status StatusManager::getStatus() {
    return _status;
}

ErrorCode StatusManager::getErrorCode() {
    return _errorCode;
}

void StatusManager::setStatus(Status newStatus) {
    if (newStatus == MEASURING) {
        _startedMeasuring = millis();
    }

    else if (newStatus == SHOWING_ERROR) {
        _startedShowing = millis();
    }

    else if (newStatus == SHOWING_RESULT) {
        _startedShowing = millis();
    }

    _status = newStatus;
}

void StatusManager::setErrorCode(ErrorCode newErrorCode) {
    _errorCode = newErrorCode;
}

bool StatusManager::isDoneMeasuring() {
    if (_status != MEASURING) return true;
    return millis() - _startedMeasuring > _measuring_sec * 1000;
}

bool StatusManager::isDoneShowingError() {
    if (_status != SHOWING_ERROR) return true;
    return millis() - _startedShowing > _showing_error_sec * 1000;
}

int StatusManager::getMeasuringTimeLeft() {
    if (_status != MEASURING) return 0;
    return _measuring_sec - (millis() - _startedMeasuring) / 1000;
}