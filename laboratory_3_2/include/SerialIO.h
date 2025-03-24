#ifndef SERIAL_IO_H
#define SERIAL_IO_H

#include <Arduino.h>

class SerialIO {
public:
    static void init(unsigned long baudRate);
    static void printMessage(const char* message);
    
    // Data reporting methods
    static void printRawValue(int rawValue);
    static void printProcessedValue(int processedValue);
    static void printFilterInfo(float noiseReduction);
    static void printSystemStatus(unsigned long uptime, int memoryUsage);
    
    // Formatted reporting
    static void printDataReport(int rawValue, int processedValue, float noiseReduction);
};

#endif // SERIAL_IO_H 