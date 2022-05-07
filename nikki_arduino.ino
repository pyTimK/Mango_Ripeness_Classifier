#include <Wire.h>

#include "src/Button/Button.h"
#include "src/EthyLeneSensor/EthyLeneSensor.h"
#include "src/ExhaustFan/ExhaustFan.h"
#include "src/IRSensor/IRSensor.h"
#include "src/LCD/LCD.h"
#include "src/MyLed/MyLed.h"
#include "src/StatusManager/StatusManager.h"
#include "src/TempSensor/TempSensor.h"

//? CONSTANTS---------------------------------------------------------------------------------------------------------------------

//! PINS
#define ETHYLENE_SERIAL_RX_PIN 2
#define ETHYLENE_SERIAL_TX_PIN 3
#define START_BUTTON_PIN 4
#define EXHAUST_FAN_PIN 5
#define IR_PIN 6
#define TEMP_PIN 7
#define RED_LED_PIN 13
#define BLUE_LED_PIN 12
#define ORANGE_LED_PIN 11
#define GREEN_LED_PIN 10
#define ETHYLENE_VOLTAGE_PIN A2

//! TIME RELATED
#define MEASURING_SEC 5
#define TEMP_CHECK_INTERVAL_SEC 30
#define PPM_CHECK_INTERVAL_SEC 1
#define SHOWING_ERROR_SEC 3

//! THRESHOLDS
#define TEMP_THRESH 30.0

//! ZE03 MEASUREMENT TYPE
#define IS_ETHYLENE_MEASUREMENT_TYPE_DIGITAL false

//?-------------------------------------------------------------------------------------------------------------------------------

//! INITIALIZE CLASSES
IRSensor mangoIRSensor(IR_PIN);

Button startButton(START_BUTTON_PIN);

TempSensor tempSensor(TEMP_PIN, TEMP_THRESH, TEMP_CHECK_INTERVAL_SEC);

EthyleneSensor ethyleneSensor(ETHYLENE_SERIAL_RX_PIN, ETHYLENE_SERIAL_TX_PIN, ETHYLENE_VOLTAGE_PIN, PPM_CHECK_INTERVAL_SEC, IS_ETHYLENE_MEASUREMENT_TYPE_DIGITAL);

StatusManager statusManager(MEASURING_SEC, SHOWING_ERROR_SEC);

LCD lcd(0x27, 20, 4);

ExhaustFan exhaustFan(EXHAUST_FAN_PIN);

MyLed redLed(RED_LED_PIN);
MyLed blueLed(BLUE_LED_PIN);
MyLed orangeLed(ORANGE_LED_PIN);
MyLed greenLed(GREEN_LED_PIN);

void setup() {
    Serial.begin(9600);

    //! INPUTS
    mangoIRSensor.begin();
    startButton.begin();
    tempSensor.begin();
    ethyleneSensor.begin();

    //! OUTPUTS
    lcd.begin();
    exhaustFan.begin();
    redLed.begin();
    blueLed.begin();
    orangeLed.begin();
    greenLed.begin();
}

void loop() {
    Status status = statusManager.getStatus();
    ErrorCode errorCode = statusManager.getErrorCode();
    bool mangoDetected = mangoIRSensor.objectDetected();

    //! INPUTS
    tempUpdate(status);
    ethyleneUpdate(status);

    //! PROCESSING
    statusUpdate(status, errorCode, mangoDetected);

    //! OUTPUTS
    status = statusManager.getStatus();
    errorCode = statusManager.getErrorCode();
    const float ppm = ethyleneSensor.value;
    const int measuringTimeLeft = statusManager.getMeasuringTimeLeft();

    LCDUpdate(status, errorCode, ppm, measuringTimeLeft);
    exhaustFan.onIf(status == CALIBRATING);

    redLed.onIf(status == CALIBRATING);
    blueLed.onIf(status == MANGO_DETECTION);
    orangeLed.onIf(status == MEASURING);
    greenLed.onIf(status == SHOWING_RESULT);
}

//! INPUTS
void tempUpdate(Status status) {
    switch (status) {
        case CALIBRATING:
            tempSensor.measure();
            break;
    }
}

void ethyleneUpdate(Status status) {
    switch (status) {
        case MEASURING:
            ethyleneSensor.intervalMeasure();
            break;

        case SHOWING_RESULT:
            ethyleneSensor.intervalMeasure();
            break;
    }
}

//! OUTPUTS
void LCDUpdate(Status status, ErrorCode errorCode, float ppm, int measuringTimeLeft) {
    switch (status) {
        case CALIBRATING:
            lcd.print(lcd.center("Calibrating"), 1);
            lcd.print(lcd.center(String(tempSensor.value, 1) + " " + String((char)223) + "C"), 3);
            break;

        case MANGO_DETECTION:
            lcd.print(lcd.center("Waiting for mango"), 2);
            break;

        case SHOWING_ERROR:
            switch (errorCode) {
                case NO_MANGO:
                    lcd.print(lcd.center("No mango"), 2);
                    break;
            }
            break;

        case MEASURING:
            lcd.print(lcd.center("Measuring"), 1);
            lcd.print(lcd.center(String(measuringTimeLeft) + " s"), 3);
            break;

        case SHOWING_RESULT:
            lcd.print(lcd.center(String(ppm) + " ppm"), 2);
            lcd.print(lcd.center(getClassification(ppm)), 4);
            break;
    }
}

//! PROCESSING
void statusUpdate(Status status, ErrorCode errorCode, bool mangoDetected) {
    const bool buttonPressed = startButton.isPressed();

    switch (status) {
        case IDLE:

            if (buttonPressed) {
                setStatus(CALIBRATING);
            }
            break;

        case CALIBRATING:
            if (tempSensor.withinThreshold()) {
                setStatus(MANGO_DETECTION);
            }
            break;

        case MANGO_DETECTION:
            if (mangoDetected) {
                setStatus(MEASURING);
            } else {
                setStatus(SHOWING_ERROR);
                setErrorCode(NO_MANGO);
            }
            break;

        case SHOWING_ERROR:
            if (statusManager.isDoneShowingError()) {
                setStatus(IDLE);
            }
            break;

        case MEASURING:
            if (statusManager.isDoneMeasuring()) setStatus(SHOWING_RESULT);
            break;

        case SHOWING_RESULT:
            if (!mangoDetected) setStatus(IDLE);
            break;
    }
}

void setStatus(Status newStatus) {
    statusManager.setStatus(newStatus);
    lcd.clear();
}

void setErrorCode(ErrorCode newErrorCode) {
    statusManager.setErrorCode(newErrorCode);
}
