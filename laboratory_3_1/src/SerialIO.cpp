#include "SerialIO.h"

void SerialIO::init(unsigned long baudRate) {
    Serial.begin(baudRate);
    while (!Serial) {
        ; // Wait for serial port to connect
    }
    Serial.println("Serial communication initialized");
}

void SerialIO::printMessage(const char* message) {
    Serial.println(message);
}

void SerialIO::printSensorData(int distance) {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
} 