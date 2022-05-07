#include "LCD.h"

#include "../StatusManager/StatusManager.h"

LCD::LCD(int addr, int cols, int rows) : _lcd(addr, cols, rows) {}

void LCD::begin() {
    _lcd.begin();
    _lcd.backlight();
    _lcd.clear();
}

void LCD::clear() {
    for (int i = 1; i < 5; i++) {
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
