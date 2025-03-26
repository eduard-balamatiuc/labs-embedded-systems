#include "DHT11Sensor.h"

DHT11Sensor::DHT11Sensor(uint8_t pin) : pin(pin) {
    dht = new DHT(pin, DHT_TYPE);
}

DHT11Sensor::~DHT11Sensor() {
    delete dht;
}

void DHT11Sensor::init() {
    dht->begin();
}

float DHT11Sensor::readTemperature() {
    float temp = dht->readTemperature();
    if (isnan(temp)) {
        return -999.0;  // Error indicator
    }
    return temp;
}

float DHT11Sensor::readHumidity() {
    float humidity = dht->readHumidity();
    if (isnan(humidity)) {
        return -999.0;  // Error indicator
    }
    return humidity;
}

float DHT11Sensor::adcToVoltage(int adcValue) {
    // DHT11 is digital but we include this for completeness
    // Assuming 5V reference and 10-bit ADC
    return (adcValue * 5.0) / 1023.0;
}

float DHT11Sensor::voltageToTemperature(float voltage) {
    // DHT11 gives direct temperature values, this is just for demonstration
    // Typically would be a calibration formula
    return voltage * 10.0; // Example conversion
}

float DHT11Sensor::voltageToHumidity(float voltage) {
    // DHT11 gives direct humidity values, this is just for demonstration
    // Typically would be a calibration formula
    return voltage * 20.0; // Example conversion
} 