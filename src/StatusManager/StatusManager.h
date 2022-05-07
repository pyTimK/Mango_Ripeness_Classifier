#ifndef StatusManager_h
#define StatusManager_h

#include "Arduino.h"

enum Status { CALIBRATING,
              IDLE,
              MEASURING,
              SHOWING_RESULT,
              SHOWING_ERROR,
};

enum ErrorCode { NO_MANGO };

class StatusManager {
   public:
    StatusManager(int measuring_sec, int showing_error_sec);
    void update(bool mangoDetected, bool buttonPressed);
    int getMeasuringTimeLeft();
    Status getStatus();
    ErrorCode getErrorCode();
    void setStatus(Status newStatus);
    void setErrorCode(ErrorCode newErrorCode);
    bool isDoneMeasuring();
    bool isDoneShowingError();

   private:
    Status _status = CALIBRATING;
    ErrorCode _errorCode = NO_MANGO;
    unsigned long _startedMeasuring = millis();
    unsigned long _startedShowing = millis();
    int _measuring_sec;
    int _showing_error_sec;
};

#endif