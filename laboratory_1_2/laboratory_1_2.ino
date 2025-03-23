#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include "printf_init.h"
#include "led_control.h"

#define GREEN_LED 13  // Using onboard LED for valid code feedback
#define RED_LED 12    // External LED for invalid code feedback

// LCD Setup (I2C address might be 0x27 or 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Keypad Setup
const byte KEYPAD_ROWS = 4;
const byte KEYPAD_COLS = 4;
char keys[KEYPAD_ROWS][KEYPAD_COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};
byte rowPins[KEYPAD_ROWS] = {9, 8, 7, 6};
byte colPins[KEYPAD_COLS] = {5, 4, 3, 2};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);

String enteredCode = ""; // Stores the entered code
const String correctCode = "1234"; // Predefined correct code

void setup() {
    Serial.begin(115200);
    printf_init(); // Enable printf and scanf
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);

    // LCD Reset Sequence
    Wire.begin();
    lcd.begin(16, 2);
    lcd.noBacklight();  // Turn off backlight to force reset
    delay(100);         // Short delay to ensure reset
    lcd.backlight();    // Turn backlight back on
    lcd.clear();        // Clear any previous text
    
    // Startup Message
    lcd.setCursor(0, 0);
    printf("Initializing...");
    delay(2000); // Pause for 2 seconds

    lcd.clear();
    lcd.setCursor(0, 0);
    printf("Keypad System");
    lcd.setCursor(0, 1);
    printf("Ready to Use");
    delay(2000);

    lcd.clear();
    lcd.setCursor(0, 0);
    printf("Enter Code:");
}


void validateCode() {
    lcd.clear();
    lcd.setCursor(0, 0);

    if (enteredCode == correctCode) {
        printf("Access Granted!");
        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(RED_LED, LOW);
        printf("Valid Code Entered: %s\n", enteredCode.c_str());
    } else {
        printf("Access Denied!");
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(RED_LED, HIGH);
        printf("Invalid Code Entered: %s\n", enteredCode.c_str());
    }

    delay(3000); // Show message for 3 seconds
    lcd.clear();
    lcd.setCursor(0, 0);
    printf("Enter Code:");
    enteredCode = ""; // Reset for next input
}

void loop() {
    char key = keypad.getKey();

    if (key) {
        printf("Key Pressed: %c\n", key);
        
        if (key == '#') { // Enter key, validate code
            validateCode();
        } else if (key == '*') { // Reset input
            enteredCode = "";
            lcd.clear();
            lcd.setCursor(0, 0);
            printf("Enter Code:");
        } else {
            enteredCode += key;
            lcd.setCursor(enteredCode.length() - 1, 1);
            printf(key); // Directly display key on LCD
        }
    }
}
