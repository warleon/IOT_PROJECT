#include <Arduino.h>
#include <App.hpp>

App app("Incuvator",{32,33,25,26});


void setup() {
  Serial.begin(9600);
  App::config_t config;
  config.SSID = "";
  config.password = "";
  config.mqttServer = "";
  config.mqttUser = "";
  config.mqttPassword ="";
  config.mqttPort = 1883;
  app.setup(config);
}

void loop() {
  app.tick();
}
