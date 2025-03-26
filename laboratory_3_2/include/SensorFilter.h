#ifndef SENSOR_FILTER_H
#define SENSOR_FILTER_H

#include <Arduino.h>

class SensorFilter {
public:
    // Salt and pepper filter (median filter) implementation
    static float saltAndPepperFilter(float* values, int size);
    
    // Weighted average filter implementation
    static float weightedAverageFilter(float* values, int size, float* weights);
    
    // Apply saturation between min and max values
    static float applySaturation(float value, float min, float max);
};

#endif // SENSOR_FILTER_H 