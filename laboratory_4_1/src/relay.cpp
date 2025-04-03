#include "relay.h"
#include <Arduino.h>

Relay::Relay(uint8_t pin) {
    this->pin = pin;
    this->state = false;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    Serial.print("Relay initialized on pin ");
    Serial.println(pin);
}

void Relay::turnOn() {
    digitalWrite(pin, LOW);
    state = true;
    Serial.println("Relay state: ON");
}

void Relay::turnOff() {
    digitalWrite(pin, HIGH);
    state = false;
    Serial.println("Relay state: OFF");
}

bool Relay::getState() const {
    return state;
} 