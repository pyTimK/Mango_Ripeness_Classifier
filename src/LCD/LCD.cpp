#include "LCD.h"

#include "../StatusManager/StatusManager.h"

LCD::LCD(int addr, int cols, int rows) : _lcd(addr, cols, rows) {}

void LCD::begin() {
    _lcd.begin();
    _lcd.backlight();
    _lcd.clear();
}

void LCD::update(Status status, float ppm, int measuringTimeLeft, ErrorCode errorCode) {
    switch (status) {
        case CALIBRATING:
            print(center("Calibrating"), 1);
            print(center(String(tempSensor.value, 1) + " " + String((char)223) + "C"), 3);
            break;

        case IDLE:
            break;

        case SHOWING_ERROR:
            switch (errorCode) {
                case NO_MANGO:
                    print(center("No mango"), 2);
                    break;
            }
            break;

        case MEASURING:
            print(center("Measuring"), 1);
            print(center(String(measuringTimeLeft) + " s"), 3);
            break;

        case SHOWING_RESULT:
            print(center(String(ppm) + " ppm"), 2);
            print(center(getClassification(ppm)), 4);
            break;
    }
}

void LCD::clear() {
    for (int i = 0; i < 4; i++) {
        print("                    ", i);
    }
}

void LCD::print(String str, int line) {
    const int strLen = min(str.length(), 20);

    for (int i = 0; i < strLen; i++) {
        if (str[i] == _showing[line - 1][i]) continue;

        _showing[line - 1][i] = str[i];
        _lcd.setCursor(i, line - 1);
        _lcd.print(str[i]);
    }
}
String LCD::center(String s) {
    const int strLen = s.length();
    if (strLen >= 20) return s.substring(0, 20);

    const int initialSpacesLen = (20 - strLen) / 2;

    String spaces = "";
    for (int i = 0; i < initialSpacesLen; i++) spaces += " ";

    return spaces + s;
}