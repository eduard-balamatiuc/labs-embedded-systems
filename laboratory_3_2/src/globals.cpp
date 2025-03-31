// globals.cpp
#include "globals.h"

// Define the LCD dimensions
int lcdColumns = 16;
int lcdRows = 2;

// Initialize the LCD object with I2C address 0x27
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);