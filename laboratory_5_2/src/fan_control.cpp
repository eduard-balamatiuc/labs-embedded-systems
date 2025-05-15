#include "fan_control.h"
#include <Arduino.h>

static int fanPwmPin = 9;  // Default PWM pin
static int currentSpeed = 0;

void FanControl_Init(int pwmPin) {
  fanPwmPin = pwmPin;
  pinMode(fanPwmPin, OUTPUT);
  currentSpeed = 0;
  
  // Set PWM frequency to ~25kHz (for Timer1, pins 9 and 10)
  // For Arduino Uno/Nano, Timer1 controls pins 9 and 10
  if (fanPwmPin == 9 || fanPwmPin == 10) {
    // Set Timer1 to fast PWM mode (mode 14)
    // WGM13:0 = 1110 for fast PWM mode with ICR1 as TOP
    TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11);
    TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS10);  // No prescaler
    
    // Set TOP value for Timer1 to get ~25kHz
    // 16MHz / 25kHz = 640
    ICR1 = 640;
    
    // Initialize with 0 duty cycle directly
    if (fanPwmPin == 9) {
      OCR1A = 0;
    } else {
      OCR1B = 0;
    }
  } else {
    // For pins other than 9/10, use standard analogWrite
    analogWrite(fanPwmPin, currentSpeed);
  }
}

void FanControl_SetSpeed(int speed) {
  // Constrain speed to valid PWM range
  speed = constrain(speed, 0, 255);
  
  // Set PWM value
  if (fanPwmPin == 9) {
    // For pin 9 (OC1A), scale speed from 0-255 to 0-ICR1
    OCR1A = map(speed, 0, 255, 0, ICR1);
  } else if (fanPwmPin == 10) {
    // For pin 10 (OC1B), scale speed from 0-255 to 0-ICR1
    OCR1B = map(speed, 0, 255, 0, ICR1);
  } else {
    // For other pins, use standard analogWrite
    analogWrite(fanPwmPin, speed);
  }
  
  currentSpeed = speed;
}

int FanControl_GetSpeed() {
  return currentSpeed;
}