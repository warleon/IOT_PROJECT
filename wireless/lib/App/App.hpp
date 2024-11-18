#pragma once

#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT_MODULE.hpp>
#include <RELAY_MODULE.hpp>
#include <SERVO_MODULE.hpp>
#include <Time.hpp>

class App
{
public:
    typedef struct {
        String SSID;
        String password;
        String mqttServer;
        String mqttUser;
        String mqttPassword;
        int mqttPort;
    } config_t;
    typedef struct {
        int dht_pin;
        int servo_pin;
        int fan_pin;
        int light_pin;
    } periferals_info_t;

    App(String,periferals_info_t);
    void setup(config_t);
    void tick();
    ~App();
private:
    static App *instance;

    String name;
    WiFiClient wifi;
    PubSubClient client;

    DHT_MODULE sensor;
    SERVO_MODULE servo;
    RELAY_MODULE fan;
    RELAY_MODULE light;

    Time timer;
    void on_read(float,float);

    static void humidity_too_low(float,float);
    static void humidity_too_high(float,float);
    static void humidity_in_range(float,float);
    static void temperature_too_low(float,float);
    static void temperature_too_high(float,float);
    static void temperature_in_range(float,float);
    static void sensor_error(String);
};