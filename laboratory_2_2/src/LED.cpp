#include "LED.h"

LED::LED(uint8_t pin) {
    this->pin = pin;
    this->state = false;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);  // Ensure LED is off initially
}

void LED::turnOn() {
    digitalWrite(pin, HIGH);
    state = true;
}

void LED::turnOff() {
    digitalWrite(pin, LOW);
    state = false;
}

void LED::toggle() {
    state = !state;
    digitalWrite(pin, state ? HIGH : LOW);
}

void LED::blink(uint8_t times, uint16_t onTime, uint16_t offTime) {
    for (uint8_t i = 0; i < times; i++) {
        turnOn();
        vTaskDelay(pdMS_TO_TICKS(onTime));
        turnOff();
        if (i < times - 1) {  // No delay after the last blink
            vTaskDelay(pdMS_TO_TICKS(offTime));
        }
    }
} 