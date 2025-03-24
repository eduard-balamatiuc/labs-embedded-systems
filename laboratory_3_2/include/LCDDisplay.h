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
    
    // Display methods
    void displayRawValue(int rawValue);
    void displayProcessedValue(int processedValue);
    void displayFilterInfo(float noiseReduction);
    void clearLine(uint8_t line);
};

#endif // LCD_DISPLAY_H 