#include <Arduino.h>
#include <App.hpp>

App app("Incubator",{32,33,25,26});


void setup() {
  Serial.begin(9600);
  App::config_t config;
  config.SSID = "Warleon wifi";
  config.password = "12345678";
  //config.mqttServer = "192.168.229.226";
  config.mqttServer = "192.168.65.226";
  config.mqttUser = "";
  config.mqttPassword ="";
  config.mqttPort = 1883;
  app.setup(config);
}

void loop() {
  app.tick();
}
