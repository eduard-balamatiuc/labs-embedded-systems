#include "SignalProcessor.h"

SignalProcessor::SignalProcessor() {
    // Initialize buffers
    for (int i = 0; i < BUFFER_SIZE; i++) {
        rawBuffer[i] = 0;
        filteredBuffer[i] = 0;
    }
    
    currentIndex = 0;
    
    // Set salt and pepper threshold (cm)
    saltPepperThreshold = 10;
    
    // Initialize weighted average weights (sum should be 1.0)
    // More recent values have higher weights
    weightedAvgWeights[0] = 0.30f;  // Most recent
    weightedAvgWeights[1] = 0.25f;
    weightedAvgWeights[2] = 0.15f;
    weightedAvgWeights[3] = 0.10f;
    weightedAvgWeights[4] = 0.05f;
    weightedAvgWeights[5] = 0.05f;
    weightedAvgWeights[6] = 0.04f;
    weightedAvgWeights[7] = 0.03f;
    weightedAvgWeights[8] = 0.02f;
    weightedAvgWeights[9] = 0.01f;  // Oldest
}

void SignalProcessor::addReading(int rawReading) {
    // Add new reading to the front of the buffer
    for (int i = BUFFER_SIZE - 1; i > 0; i--) {
        rawBuffer[i] = rawBuffer[i - 1];
    }
    rawBuffer[0] = rawReading;
    
    // Increment current index with wrap-around
    currentIndex = (currentIndex + 1) % BUFFER_SIZE;
}

int SignalProcessor::applySaltPepperFilter() {
    // Skip if we don't have enough data yet
    if (rawBuffer[2] == 0) {
        return rawBuffer[0];
    }
    
    // Check if current value is an outlier
    int median = 0;
    
    // Simple median of 3 approach for this example
    int a = rawBuffer[0];
    int b = rawBuffer[1];
    int c = rawBuffer[2];
    
    // Find median of a, b, c
    if ((a <= b && b <= c) || (c <= b && b <= a)) median = b;
    else if ((b <= a && a <= c) || (c <= a && a <= b)) median = a;
    else median = c;
    
    // If the difference between current reading and median is greater than threshold,
    // replace with the median
    if (abs(rawBuffer[0] - median) > saltPepperThreshold) {
        return median;
    }
    
    return rawBuffer[0];
}

int SignalProcessor::applyWeightedAverageFilter() {
    float weightedSum = 0.0f;
    
    // Calculate weighted sum
    for (int i = 0; i < BUFFER_SIZE; i++) {
        // Stop if we reach uninitialized values
        if (rawBuffer[i] == 0 && i > 0) break;
        
        weightedSum += filteredBuffer[i] * weightedAvgWeights[i];
    }
    
    return (int)weightedSum;
}

float SignalProcessor::adcToVoltage(int adcValue) {
    // For ultrasonic sensor, this is not necessary as it returns distance directly
    // This would be needed for analog sensors
    return (float)adcValue;
}

float SignalProcessor::voltageToDistance(float voltage) {
    // For ultrasonic sensor, this is not necessary as it returns distance directly
    // This would be needed for analog sensors
    return voltage;
}

int SignalProcessor::saturate(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

int SignalProcessor::processReading(int rawReading) {
    // Step 1: Add the raw reading to the buffer
    addReading(rawReading);
    
    // Step 2: Apply salt and pepper filter
    int filteredValue = applySaltPepperFilter();
    
    // Store filtered value in filtered buffer
    for (int i = BUFFER_SIZE - 1; i > 0; i--) {
        filteredBuffer[i] = filteredBuffer[i - 1];
    }
    filteredBuffer[0] = filteredValue;
    
    // Step 3: Apply weighted moving average filter
    filteredValue = applyWeightedAverageFilter();
    
    // Step 4: Convert and apply saturation (limit to reasonable range for ultrasonic)
    // Typical HC-SR04 range is 2cm to 400cm
    filteredValue = saturate(filteredValue, 2, 400);
    
    return filteredValue;
}

int SignalProcessor::getProcessedValue() {
    return filteredBuffer[0];
}

float SignalProcessor::getNoiseReductionPercentage() {
    // Calculate how much noise has been reduced by comparing raw and filtered values
    float sumDifference = 0.0f;
    float sumRaw = 0.1f;  // Avoid division by zero
    
    // Use only the valid data points
    int validPoints = 0;
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (rawBuffer[i] == 0 && i > 0) break;
        
        sumDifference += abs(rawBuffer[i] - filteredBuffer[i]);
        sumRaw += abs(rawBuffer[i]);
        validPoints++;
    }
    
    // Calculate percentage of noise reduction
    float noisePercentage = (sumDifference / sumRaw) * 100.0f;
    
    // Cap it to reasonable values (0-50%)
    if (noisePercentage > 50.0f) noisePercentage = 50.0f;
    if (noisePercentage < 0.0f) noisePercentage = 0.0f;
    
    return noisePercentage;
} 