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
    lcd->print("DHT11 Sensor");
    lcd->setCursor(0, 1);
    lcd->print("Initializing...");
}

void LCDDisplay::displaySensorData(const char* data) {
    lcd->setCursor(0, 1);
    lcd->print("                "); // Clear line
    lcd->setCursor(0, 1);
    lcd->print(data);
}

void LCDDisplay::displaySystemState(const char* state) {
    lcd->setCursor(0, 0);
    lcd->print("                "); // Clear line
    lcd->setCursor(0, 0);
    lcd->print(state);
} 