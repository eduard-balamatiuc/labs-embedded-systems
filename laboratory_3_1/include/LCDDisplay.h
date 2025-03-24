#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class LCDDisplay {
private:
    LiquidCrystal_I2C* lcd;
    
public:
    LCDDisplay(uint8_t address, uint8_t columns, uint8_t rows);
    ~LCDDisplay();
    void init();
    void displaySensorData(int distance);
    void displaySystemState(const char* state);
};

#endif // LCD_DISPLAY_H 