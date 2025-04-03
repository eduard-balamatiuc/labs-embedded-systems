#ifndef RELAY_H
#define RELAY_H

#include <Arduino.h>

class Relay {
private:
    uint8_t pin;
    bool state;

public:
    Relay(uint8_t pin);
    void turnOn();
    void turnOff();
    bool getState() const;
};

#endif // RELAY_H 