#include "SerialIO.h"

void SerialIO::init(unsigned long baudRate) {
    Serial.begin(baudRate);
    while (!Serial) {
        // Wait for serial port to connect
    }
    Serial.println("Serial communication initialized");
}

void SerialIO::printMessage(const char* message) {
    Serial.println(message);
}

void SerialIO::printByteArray(uint8_t* data, size_t length) {
    Serial.print("Received data: ");
    for (size_t i = 0; i < length; i++) {
        if (data[i] == 0) {
            Serial.println(); // Print newline for byte value 0
            break;
        }
        Serial.print(data[i]);
        Serial.print(" ");
    }
    Serial.println();
} 