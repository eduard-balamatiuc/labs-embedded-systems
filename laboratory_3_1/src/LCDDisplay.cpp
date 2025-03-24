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
    lcd->print("Distance Sensor");
}

void LCDDisplay::displaySensorData(int distance) {
    lcd->setCursor(0, 1);
    lcd->print("                "); // Clear the line
    lcd->setCursor(0, 1);
    lcd->print("Distance: ");
    lcd->print(distance);
    lcd->print(" cm");
}

void LCDDisplay::displaySystemState(const char* state) {
    lcd->setCursor(0, 0);
    lcd->print("                "); // Clear the line
    lcd->setCursor(0, 0);
    lcd->print(state);
} 