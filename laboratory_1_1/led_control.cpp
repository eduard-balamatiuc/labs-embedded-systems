#include <Arduino.h>
#include <stdio.h>
#include "led_control.h"

void turnOnLED() {
    digitalWrite(13, HIGH);
    printf("LED turned on\n");
}

void turnOffLED() {
    digitalWrite(13, LOW);
    printf("LED turned off\n");
}