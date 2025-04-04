#ifndef SERIAL_IO_H
#define SERIAL_IO_H

#include <Arduino.h>

class SerialIO {
public:
    static void init(unsigned long baudRate);
    static void printMessage(const char* message);
    static void printSensorData(int distance);
};

#endif // SERIAL_IO_H 