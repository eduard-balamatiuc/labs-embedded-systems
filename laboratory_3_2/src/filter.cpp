#include "filter.h"
#include "globals.h"

// Filter arrays - static to keep persistent across calls
static float medianValues[MEDIAN_SAMPLES];
static float averageValues[AVERAGE_SAMPLES];
static int medianIndex = 0;
static int averageIndex = 0;
static bool medianBufferFilled = false;
static bool averageBufferFilled = false;

// Salt and pepper filter (median filter) to remove impulse noise
float saltPepperFilter(float newValue) {
  // Add new reading to the array
  medianValues[medianIndex] = newValue;
  medianIndex = (medianIndex + 1) % MEDIAN_SAMPLES;
  
  if (medianIndex == 0) {
    medianBufferFilled = true;
  }
  
  // Create a copy for sorting
  float tempValues[MEDIAN_SAMPLES];
  for (int i = 0; i < MEDIAN_SAMPLES; i++) {
    tempValues[i] = medianValues[i];
  }
  
  // Sort values (insertion sort - more efficient for small arrays)
  for (int i = 1; i < MEDIAN_SAMPLES; i++) {
    float key = tempValues[i];
    int j = i - 1;
    while (j >= 0 && tempValues[j] > key) {
      tempValues[j + 1] = tempValues[j];
      j--;
    }
    tempValues[j + 1] = key;
  }
  
  // Return median value
  if (medianBufferFilled) {
    return tempValues[MEDIAN_SAMPLES / 2];
  } else {
    // If buffer not filled yet, calculate average of available readings
    float sum = 0;
    for (int i = 0; i < medianIndex; i++) {
      sum += medianValues[i];
    }
    return (medianIndex > 0) ? sum / medianIndex : newValue;
  }
}

// Weighted average filter - gives more weight to recent readings
float weightedAverageFilter(float newValue) {
  // Store new reading
  averageValues[averageIndex] = newValue;
  averageIndex = (averageIndex + 1) % AVERAGE_SAMPLES;
  
  if (averageIndex == 0) {
    averageBufferFilled = true;
  }
  
  // Calculate weighted average
  float sum = 0;
  float weightSum = 0;
  
  int samples = averageBufferFilled ? AVERAGE_SAMPLES : averageIndex;
  
  for (int i = 0; i < samples; i++) {
    int pos = (averageIndex - 1 - i + AVERAGE_SAMPLES) % AVERAGE_SAMPLES;
    float weight = samples - i;
    sum += averageValues[pos] * weight;
    weightSum += weight;
  }
  
  return (weightSum > 0) ? sum / weightSum : newValue;
}

// Converts pulse duration to distance based on sound speed
float convertPulseToCm(long pulseTime) {
  return pulseTime * SOUND_SPEED / 2;
}

// Saturates values to keep them within valid range
float saturateValue(float value, float min, float max) {
  if (value < min) return min;
  if (value > max) return max;
  return value;
}