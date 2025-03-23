#ifndef LED_H
#define LED_H

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include "task.h"

class LED {
public:
    LED(uint8_t pin);
    void turnOn();
    void turnOff();
    void toggle();
    void blink(uint8_t times, uint16_t onTime, uint16_t offTime);

private:
    uint8_t pin;
    bool state;
};

#endif // LED_H 