#include "SensorFilter.h"

float SensorFilter::saltAndPepperFilter(float* values, int size) {
    // Create a copy of the array for sorting
    float tempValues[size];
    for (int i = 0; i < size; i++) {
        tempValues[i] = values[i];
    }
    
    // Simple bubble sort to find median
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (tempValues[j] > tempValues[j + 1]) {
                // Swap values
                float temp = tempValues[j];
                tempValues[j] = tempValues[j + 1];
                tempValues[j + 1] = temp;
            }
        }
    }
    
    // Return the median value
    if (size % 2 == 0) {
        return (tempValues[size/2 - 1] + tempValues[size/2]) / 2.0;
    } else {
        return tempValues[size/2];
    }
}

float SensorFilter::weightedAverageFilter(float* values, int size, float* weights) {
    float weightedSum = 0.0;
    float weightSum = 0.0;
    
    for (int i = 0; i < size; i++) {
        weightedSum += values[i] * weights[i];
        weightSum += weights[i];
    }
    
    // Return weighted average
    return weightedSum / weightSum;
}

float SensorFilter::applySaturation(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
} 