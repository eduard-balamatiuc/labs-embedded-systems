#ifndef DHT11_SENSOR_H
#define DHT11_SENSOR_H

#include <Arduino.h>
#include <DHT.h>

#define DHT_TYPE DHT11  // Type of DHT sensor

class DHT11Sensor {
private:
    uint8_t pin;
    DHT* dht;
    
public:
    DHT11Sensor(uint8_t pin);
    ~DHT11Sensor();
    void init();
    float readTemperature();
    float readHumidity();
    
    // Conversion methods for the sensor values
    float adcToVoltage(int adcValue);
    float voltageToTemperature(float voltage);
    float voltageToHumidity(float voltage);
};

#endif // DHT11_SENSOR_H 