// globals.cpp
#include "globals.h"

// Define the variables here (only once)
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);