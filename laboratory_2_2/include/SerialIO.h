#ifndef SERIAL_IO_H
#define SERIAL_IO_H

#include <Arduino.h>

class SerialIO {
public:
    static void init(unsigned long baudRate = 9600);
    static void printMessage(const char* message);
    static void printByteArray(uint8_t* data, size_t length);
};

#endif // SERIAL_IO_H 