#include "LCDDisplay.h"

LCDDisplay::LCDDisplay(uint8_t address, uint8_t columns, uint8_t rows) {
    lcd = new LiquidCrystal_I2C(address, columns, rows);
}

LCDDisplay::~LCDDisplay() {
    delete lcd;
}

void LCDDisplay::init() {
    lcd->init();
    lcd->backlight();
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print("Sensor System");
}

void LCDDisplay::clearLine(uint8_t line) {
    lcd->setCursor(0, line);
    for (int i = 0; i < 16; i++) {
        lcd->print(" ");
    }
    lcd->setCursor(0, line);
}

void LCDDisplay::displayRawValue(int rawValue) {
    // Display raw value on first line
    clearLine(0);
    lcd->print("Raw: ");
    lcd->print(rawValue);
    lcd->print(" cm");
}

void LCDDisplay::displayProcessedValue(int processedValue) {
    // Display processed value on second line
    clearLine(1);
    lcd->print("Filt: ");
    lcd->print(processedValue);
    lcd->print(" cm");
}

void LCDDisplay::displayFilterInfo(float noiseReduction) {
    // Temporarily show filter info on the second line
    clearLine(1);
    lcd->print("NR: ");
    lcd->print(noiseReduction, 1); // 1 decimal place
    lcd->print("%");
} 