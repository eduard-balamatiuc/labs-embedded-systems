#include "SerialIO.h"

void SerialIO::init(unsigned long baudRate) {
    Serial.begin(baudRate);
    while (!Serial) {
        ; // Wait for serial port to connect
    }
    Serial.println(F("Serial communication initialized"));
}

void SerialIO::printMessage(const char* message) {
    Serial.println(message);
}

void SerialIO::printMessage(const __FlashStringHelper* message) {
    Serial.println(message);
} 