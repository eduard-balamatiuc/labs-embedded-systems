#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include <Arduino.h>

class UltrasonicSensor {
private:
    uint8_t trigPin;
    uint8_t echoPin;
    
public:
    UltrasonicSensor(uint8_t trigPin, uint8_t echoPin);
    void init();
    int readDistance();
};

#endif // ULTRASONIC_SENSOR_H 