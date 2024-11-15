#pragma once
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Time.hpp>

class DHT_MODULE
{
public:
    enum EVENT{
        HUMIDITY_TOO_LOW=0,
        HUMIDITY_TOO_HIGH,
        HUMIDITY_IN_RANGE,
        TEMPERATURE_TOO_LOW,
        TEMPERATURE_TOO_HIGH,
        TEMPERATURE_IN_RANGE,
        ERROR
    };
    typedef void (*callback_t)(float,float);
    typedef void (*error_callback_t)(String);
    typedef struct {
        float lower;
        float upper;
    }range_t;

    //pin,humidity range,temperature range
    DHT_MODULE(int,range_t,range_t);
    void setup();
    void tick();
    ~DHT_MODULE();

    callback_t on[6] = {};
    error_callback_t on_error = nullptr;

private:
    DHT_Unified module;
    range_t humidity_range;
    range_t temperature_range;
    int delayMS;
    Time time;
};