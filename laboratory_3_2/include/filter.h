#pragma once

// Function prototypes for filtering operations
float saltPepperFilter(float newValue);
float weightedAverageFilter(float newValue);
float convertPulseToCm(long pulseTime);
float saturateValue(float value, float min, float max);