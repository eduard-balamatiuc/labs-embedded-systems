#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// LCD configuration
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

// Ultrasonic sensor pins
const int trigPin = 5;
const int echoPin = 18;

// Sound speed constant
#define SOUND_SPEED 0.034

// Filtering constants
#define MEDIAN_SAMPLES 5    // Salt and pepper filter samples
#define AVERAGE_SAMPLES 5   // Weighted average filter samples

// Saturation limits for distance (typical HC-SR04 range: 2-400cm)
#define MIN_DISTANCE_CM 2.0
#define MAX_DISTANCE_CM 400.0

// Variables
long duration;
float rawDistanceCm;
float filteredDistanceCm;

// Arrays for filtering
float medianValues[MEDIAN_SAMPLES];
float averageValues[AVERAGE_SAMPLES];

void setup() {
  Serial.begin(115200);
  
  // Ultrasonic sensor setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // LCD setup
  lcd.init();
  lcd.backlight();
  
  // Initialize filter arrays
  for (int i = 0; i < MEDIAN_SAMPLES; i++) {
    medianValues[i] = 0;
  }
  
  for (int i = 0; i < AVERAGE_SAMPLES; i++) {
    averageValues[i] = 0;
  }
  
  Serial.println("Ultrasonic Sensor with Signal Conditioning");
}

// Salt and pepper filter (median filter) to remove impulse noise
float saltPepperFilter(float newValue) {
  static int index = 0;
  static bool bufferFilled = false;
  
  // Add new reading to the array
  medianValues[index] = newValue;
  index = (index + 1) % MEDIAN_SAMPLES;
  
  if (index == 0) {
    bufferFilled = true;
  }
  
  // Create a copy for sorting
  float tempValues[MEDIAN_SAMPLES];
  memcpy(tempValues, medianValues, sizeof(medianValues));
  
  // Sort values (bubble sort)
  for (int i = 0; i < MEDIAN_SAMPLES - 1; i++) {
    for (int j = 0; j < MEDIAN_SAMPLES - i - 1; j++) {
      if (tempValues[j] > tempValues[j + 1]) {
        float temp = tempValues[j];
        tempValues[j] = tempValues[j + 1];
        tempValues[j + 1] = temp;
      }
    }
  }
  
  // Return median value
  if (bufferFilled) {
    return tempValues[MEDIAN_SAMPLES / 2];
  } else {
    // If buffer not filled yet, calculate average of available readings
    float sum = 0;
    for (int i = 0; i < index; i++) {
      sum += medianValues[i];
    }
    return (index > 0) ? sum / index : newValue;
  }
}

// Weighted average filter - gives more weight to recent readings
float weightedAverageFilter(float newValue) {
  static int index = 0;
  static bool bufferFilled = false;
  
  // Store new reading
  averageValues[index] = newValue;
  index = (index + 1) % AVERAGE_SAMPLES;
  
  if (index == 0) {
    bufferFilled = true;
  }
  
  // Calculate weighted average
  float sum = 0;
  float weightSum = 0;
  
  int samples = bufferFilled ? AVERAGE_SAMPLES : index;
  
  for (int i = 0; i < samples; i++) {
    int pos = (index - 1 - i + AVERAGE_SAMPLES) % AVERAGE_SAMPLES;
    // Weight decreases for older samples
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

void loop() {
  // Send ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Read the pulse duration
  duration = pulseIn(echoPin, HIGH);
  
  // Convert to raw distance
  rawDistanceCm = convertPulseToCm(duration);
  
  // Apply saturation to raw reading
  rawDistanceCm = saturateValue(rawDistanceCm, MIN_DISTANCE_CM, MAX_DISTANCE_CM);
  
  // Apply filters in sequence
  float medianFiltered = saltPepperFilter(rawDistanceCm);
  filteredDistanceCm = weightedAverageFilter(medianFiltered);
  
  // Display on Serial Monitor
  Serial.print("Raw (cm): ");
  Serial.print(rawDistanceCm, 1);
  Serial.print("\tFiltered (cm): ");
  Serial.println(filteredDistanceCm, 1);
  
  // Display on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Filtered: ");
  lcd.print(filteredDistanceCm, 1);
  lcd.print("cm");
  
  lcd.setCursor(0, 1);
  lcd.print("Raw: ");
  lcd.print(rawDistanceCm, 1);
  lcd.print("cm");
  
  delay(100);  // Faster refresh rate for better filtering
}