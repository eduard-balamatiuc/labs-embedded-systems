#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include "printf_init.h"
#include "led_control.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Redirect printf to Serial
int serial_putchar(char c, FILE* f) {
    if (c == '\n') Serial.write('\r');
    Serial.write(c);
    lcd.print(c);
}

// Redirect scanf to Serial
int serial_getchar(FILE* f) {
    while (!Serial.available());
    return Serial.read();
}

void printf_init() {
    fdevopen(&serial_putchar, &serial_getchar);
}