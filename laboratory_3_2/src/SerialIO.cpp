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

void SerialIO::printRawValue(int rawValue) {
    Serial.print("Raw Value: ");
    Serial.print(rawValue);
    Serial.println(" cm");
}

void SerialIO::printProcessedValue(int processedValue) {
    Serial.print("Processed Value: ");
    Serial.print(processedValue);
    Serial.println(" cm");
}

void SerialIO::printFilterInfo(float noiseReduction) {
    Serial.print("Noise Reduction: ");
    Serial.print(noiseReduction, 1);
    Serial.println("%");
}

void SerialIO::printSystemStatus(unsigned long uptime, int memoryUsage) {
    Serial.print("Uptime: ");
    Serial.print(uptime / 1000);
    Serial.print(" sec, Free Mem: ");
    Serial.print(memoryUsage);
    Serial.println(" bytes");
}

void SerialIO::printDataReport(int rawValue, int processedValue, float noiseReduction) {
    Serial.println("====== SENSOR DATA REPORT ======");
    Serial.print("Raw Reading: ");
    Serial.print(rawValue);
    Serial.println(" cm");
    
    Serial.print("Filtered Reading: ");
    Serial.print(processedValue);
    Serial.println(" cm");
    
    Serial.print("Noise Reduction: ");
    Serial.print(noiseReduction, 1);
    Serial.println("%");
    
    Serial.print("Uptime: ");
    Serial.print(millis() / 1000);
    Serial.println(" seconds");
    
    Serial.println("===============================");
} 