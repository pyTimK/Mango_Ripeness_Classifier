#ifndef LCD_h
#define LCD_h

#include <LiquidCrystal_I2C.h>

#include "Arduino.h"

class LCD {
   public:
    LCD(int addr, int cols, int rows);
    void begin();
    void update(Status status, float ppm, int measuringTimeLeft, ErrorCode errorCode);
    void clear();
    void print(String str, int line);
    String center(String s);

   private:
    LiquidCrystal_I2C _lcd;
    char _showing[4][20] = {
        "                    ",
        "                    ",
        "                    ",
        "                    ",
    };
};

#endif