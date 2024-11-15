#include <Arduino.h>
#include <DHT.hpp>

DHT::DHT(int in):module(in,DHT11)
{
}

void DHT::setup()
{
    module.begin();
    module.humidity().getSensor(&humidity);
    module.temperature().getSensor(&temperature);
    Serial.println(F("DHT11 Unified Sensor"));
    // Print temperature sensor details.
    Serial.println(F("------------------------------------"));
    Serial.println(F("Temperature Sensor"));
    Serial.print  (F("Sensor Type: ")); Serial.println(temperature.name);
    Serial.print  (F("Driver Ver:  ")); Serial.println(temperature.version);
    Serial.print  (F("Unique ID:   ")); Serial.println(temperature.sensor_id);
    Serial.print  (F("Max Value:   ")); Serial.print(temperature.max_value); Serial.println(F("°C"));
    Serial.print  (F("Min Value:   ")); Serial.print(temperature.min_value); Serial.println(F("°C"));
    Serial.print  (F("Resolution:  ")); Serial.print(temperature.resolution); Serial.println(F("°C"));
    Serial.println(F("------------------------------------"));
    // Print humidity sensor details.
    Serial.println(F("Humidity Sensor"));
    Serial.print  (F("Sensor Type: ")); Serial.println(humidity.name);
    Serial.print  (F("Driver Ver:  ")); Serial.println(humidity.version);
    Serial.print  (F("Unique ID:   ")); Serial.println(humidity.sensor_id);
    Serial.print  (F("Max Value:   ")); Serial.print(humidity.max_value); Serial.println(F("%"));
    Serial.print  (F("Min Value:   ")); Serial.print(humidity.min_value); Serial.println(F("%"));
    Serial.print  (F("Resolution:  ")); Serial.print(humidity.resolution); Serial.println(F("%"));
    Serial.println(F("------------------------------------"));
    // Set delay between sensor readings based on sensor details.
    delayMS = humidity.min_delay / 1000;
}

void DHT::loop()
{
}

DHT::~DHT()
{
}
