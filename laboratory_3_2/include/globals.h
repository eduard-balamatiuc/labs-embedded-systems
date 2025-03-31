#pragma once
#include <LiquidCrystal_I2C.h>

// Declare variables as external
extern int lcdColumns;
extern int lcdRows;
extern LiquidCrystal_I2C lcd;

// Ultrasonic sensor pins
#define TRIG_PIN 5
#define ECHO_PIN 18

// Sound speed & filtering constants
#define SOUND_SPEED 0.034
#define MEDIAN_SAMPLES 5
#define AVERAGE_SAMPLES 5
#define MIN_DISTANCE_CM 2.0
#define MAX_DISTANCE_CM 400.0