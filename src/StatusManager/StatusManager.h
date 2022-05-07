#ifndef StatusManager_h
#define StatusManager_h

#include "../LCD/LCD.h"
#include "../TempSensor/TempSensor.h"
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
    void begin(TempSensor *tempSensor, LCD *lcd);
    Status status = CALIBRATING;
    ErrorCode errorCode = NO_MANGO;
    void setStatus(Status newStatus);
    void update(bool mangoDetected, bool buttonPressed);
    int getMeasuringTimeLeft();

   private:
    TempSensor *_tempSensor;
    LCD *_lcd;
    unsigned long _startedMeasuring = millis();
    unsigned long _startedShowing = millis();
    int _measuring_sec;
    int _showing_error_sec;
    bool _isDoneMeasuring();
    bool _isDoneShowingError();
};

#endif