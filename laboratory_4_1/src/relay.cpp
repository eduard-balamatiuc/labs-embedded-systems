#include "relay.h"
#include <stdio.h>

Relay::Relay(uint8_t pin) {
    this->pin = pin;
    this->state = false;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    printf("Relay initialized on pin %d\n", pin);
}

void Relay::turnOn() {
    digitalWrite(pin, LOW);
    state = true;
    printf("Relay state: ON\n");
}

void Relay::turnOff() {
    digitalWrite(pin, HIGH);
    state = false;
    printf("Relay state: OFF\n");
}

bool Relay::getState() const {
    return state;
} 