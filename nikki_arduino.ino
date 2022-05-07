
#include <Wire.h>

#include "src/EthyLeneSensor/EthyLeneSensor.h"
#include "src/LCD/LCD.h"
#include "src/MyLed/MyLed.h"
#include "src/StatusManager/StatusManager.h"
#include "src/TempSensor/TempSensor.h"

//! PINS
#define ETHYLENE_SERIAL_RX_PIN 2
#define ETHYLENE_SERIAL_TX_PIN 3
#define START_BUTTON_PIN 4
#define FAN_PIN 5
#define IR_PIN 6
#define TEMP_PIN 7
#define RED_LED_PIN 13
#define BLUE_LED_PIN 12
#define ORANGE_LED_PIN 11
#define GREEN_LED_PIN 10
#define ETHYLENE_VOLTAGE_PIN A2

//! CONSTANTS
#define TEMP_THRESH 29.0
#define TEMP_CHECK_INTERVAL_SEC 30
#define MEASURING_SEC 5
#define SHOWING_ERROR_SEC 3
#define PPM_CHECK_INTERVAL_SEC 1

//! INITIALIZE CLASSES

// LiquidCrystal_I2C lcd(0x27, 20, 4);

LCD lcd(0x27, 20, 4);

MyLed redLed(RED_LED_PIN);
MyLed blueLed(BLUE_LED_PIN);
MyLed orangeLed(ORANGE_LED_PIN);
MyLed greenLed(GREEN_LED_PIN);

TempSensor tempSensor(TEMP_PIN, TEMP_THRESH, TEMP_CHECK_INTERVAL_SEC);

EthyleneSensor ethyleneSensor(ETHYLENE_SERIAL_RX_PIN, ETHYLENE_SERIAL_TX_PIN, ETHYLENE_VOLTAGE_PIN, PPM_CHECK_INTERVAL_SEC);

StatusManager statusManager(MEASURING_SEC, SHOWING_ERROR_SEC);

void setup() {
    Serial.begin(9600);

    // INITIALIZE ETHYLENE SENSOR
    ethyleneSensor.begin();

    // INITIALIZE TEMPERATURE SENSOR
    tempSensor.begin();

    // INITIALIZE EXHAUST FAN
    pinMode(FAN_PIN, OUTPUT);
    digitalWrite(FAN_PIN, LOW);

    // INITIALIZE INFRARED SENSOR
    pinMode(IR_PIN, INPUT);

    // INITIALIZE LEDs
    redLed.begin();
    blueLed.begin();
    orangeLed.begin();
    greenLed.begin();

    // INITIALIZE BUTTON
    pinMode(START_BUTTON_PIN, INPUT);

    // INITIALIZE LCD
    lcd.begin();

    // INITIALIE STATUS MANAGER
    statusManager.begin(&tempSensor, &lcd);
}

void loop() {
    const bool mangoDetected = checkIfMangoDetected();
    const bool buttonPressed = checkIfButtonPressed();

    Serial.print(tempSensor.value);
    Serial.print("\t");

    statusManager.update(mangoDetected, buttonPressed);
    Serial.println(tempSensor.value);

    ethylene_sensor();

    exhaust_fan();
    // Serial.println(tempSensor.value);

    //! LEDs
    redLed.onIf(statusManager.status == CALIBRATING);
    blueLed.onIf(statusManager.status == IDLE && mangoDetected);
    orangeLed.onIf(statusManager.status == MEASURING);
    greenLed.onIf(statusManager.status == SHOWING_RESULT);

    lcd.update(statusManager.status, ethyleneSensor.value, statusManager.getMeasuringTimeLeft(), statusManager.errorCode);
    LCDa();
}

void update(bool mangoDetected, bool buttonPressed) {
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

//!!!!!!!!!!!!!!!!!!!!!!!!! TO MOVE
String getClassification(float ppm) {
    if (ppm < 30) return "Not Sweet";
    if (ppm < 65) return "Mildly Sweet";
    return "Sweet";
}

void ethylene_sensor() {
    if (statusManager.status != MEASURING && statusManager.status != SHOWING_RESULT) return -1;
    ethyleneSensor.measure();
}

void exhaust_fan() {
    if (statusManager.status == CALIBRATING) {
        startFan();
    } else {
        stopFan();
    }
}

bool checkIfButtonPressed() {
    bool buttonPressed = digitalRead(START_BUTTON_PIN);
    return buttonPressed;
}

void startFan() {
    digitalWrite(FAN_PIN, HIGH);
}

void stopFan() {
    digitalWrite(FAN_PIN, LOW);
}

bool checkIfMangoDetected() {
    return !digitalRead(IR_PIN);
}
