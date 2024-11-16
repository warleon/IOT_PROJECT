#include <Arduino.h>
#include <DHT_MODULE.hpp>

DHT_MODULE::DHT_MODULE(int in,range_t hrange,range_t trange):module(in,DHT11),humidity_range(hrange),temperature_range(trange)
{
}

void DHT_MODULE::setup()
{
    module.begin();
    sensor_t humidity;
    sensor_t temperature;
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
    time = Time();
}

void DHT_MODULE::tick()
{
    time.tick();
    if(time.acc < delayMS)return;
    time.reset();
    sensors_event_t event;
    module.humidity().getEvent(&event);
    float humidity = event.relative_humidity;
    module.temperature().getEvent(&event);
    float temperature = event.temperature;
    if(isnan(humidity)){
        if(on_error) on_error("Error reading humidity!");
    }else{
        if(humidity < humidity_range.lower){
            if(on[HUMIDITY_TOO_LOW])on[HUMIDITY_TOO_LOW](humidity,temperature);
        }
        else if(humidity > humidity_range.upper){
            if(on[HUMIDITY_TOO_HIGH])on[HUMIDITY_TOO_HIGH](humidity,temperature);
        }
        else{
            if(on[HUMIDITY_IN_RANGE])on[HUMIDITY_IN_RANGE](humidity,temperature);
        }
    }
    if(isnan(temperature)){
        if(on_error) on_error("Error reading temperature!");
    }else{
        if(temperature < temperature_range.lower){
            if(on[TEMPERATURE_TOO_LOW])on[TEMPERATURE_TOO_LOW](humidity,temperature);
        }
        else if(temperature > temperature_range.upper){
            if(on[TEMPERATURE_TOO_HIGH])on[TEMPERATURE_TOO_HIGH](humidity,temperature);
        }
        else{
            if(on[TEMPERATURE_IN_RANGE])on[TEMPERATURE_IN_RANGE](humidity,temperature);
        }
    }
}

DHT_MODULE::~DHT_MODULE()
{
}
