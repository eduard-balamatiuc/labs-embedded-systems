#ifndef SIGNAL_PROCESSOR_H
#define SIGNAL_PROCESSOR_H

#include <Arduino.h>

// Buffer size for signal processing
#define BUFFER_SIZE 10

class SignalProcessor {
private:
    // Buffers for raw and processed data
    int rawBuffer[BUFFER_SIZE];
    int filteredBuffer[BUFFER_SIZE];
    int currentIndex;
    
    // Filter parameters
    int saltPepperThreshold;
    float weightedAvgWeights[BUFFER_SIZE];
    
    // Convert from digital to voltage and physical units
    float adcToVoltage(int adcValue);
    float voltageToDistance(float voltage);
    
    // Saturation function to limit values to valid range
    int saturate(int value, int min, int max);
    
public:
    SignalProcessor();
    
    // Add a new raw reading to the buffer
    void addReading(int rawReading);
    
    // Apply salt and pepper filter (remove outliers)
    int applySaltPepperFilter();
    
    // Apply weighted moving average filter
    int applyWeightedAverageFilter();
    
    // Process the raw reading through all filters and conversions
    int processReading(int rawReading);
    
    // Get the latest processed value
    int getProcessedValue();
    
    // Get filter statistics
    float getNoiseReductionPercentage();
};

#endif // SIGNAL_PROCESSOR_H 