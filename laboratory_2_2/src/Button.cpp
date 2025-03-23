#include "Button.h"

Button::Button(uint8_t pin) {
    this->pin = pin;
    pinMode(pin, INPUT_PULLUP);  // Using internal pull-up resistor
}

bool Button::isPressed() {
    return digitalRead(pin) == HIGH;
} 