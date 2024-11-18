#include <Arduino.h>
#include <DHT_MODULE.hpp>
#include <RELAY_MODULE.hpp>
#include <SERVO_MODULE.hpp>

DHT_MODULE DHT_sensor(32,{20.0f,80.0f},{0.0f,50.0f});
SERVO_MODULE servo(33);



void humidity_too_low(float,float);
void humidity_too_high(float,float);
void humidity_in_range(float,float);
void temperature_too_low(float,float);
void temperature_too_high(float,float);
void temperature_in_range(float,float);

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  DHT_sensor.setup();
  DHT_sensor.on[DHT_sensor.HUMIDITY_TOO_LOW] = humidity_too_low;
  DHT_sensor.on[DHT_sensor.HUMIDITY_TOO_HIGH] = humidity_too_high;
  DHT_sensor.on[DHT_sensor.HUMIDITY_IN_RANGE] = humidity_in_range;

  DHT_sensor.on[DHT_sensor.TEMPERATURE_TOO_LOW] = temperature_too_low;
  DHT_sensor.on[DHT_sensor.TEMPERATURE_TOO_HIGH] = temperature_too_high;
  DHT_sensor.on[DHT_sensor.TEMPERATURE_IN_RANGE] = temperature_in_range;

  servo.setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  DHT_sensor.tick();
  servo.clear();
  servo.set(0);
  servo.set(180);
  delay(1000);
}

void humidity_too_low(float humidity,float temperature){
  Serial.print(humidity);
  Serial.print(" ");
  Serial.print(temperature);
  Serial.print(" ");
  Serial.println("humidity_too_low");
}
void humidity_too_high(float humidity,float temperature){
  Serial.print(humidity);
  Serial.print(" ");
  Serial.print(temperature);
  Serial.print(" ");
  Serial.println("humidity_too_high");
}
void humidity_in_range(float humidity,float temperature){
  Serial.print(humidity);
  Serial.print(" ");
  Serial.print(temperature);
  Serial.print(" ");
  Serial.println("humidity_in_range");
}
void temperature_too_low(float humidity,float temperature){
  Serial.print(humidity);
  Serial.print(" ");
  Serial.print(temperature);
  Serial.print(" ");
  Serial.println("temperature_too_low");
}
void temperature_too_high(float humidity,float temperature){
  Serial.print(humidity);
  Serial.print(" ");
  Serial.print(temperature);
  Serial.print(" ");
  Serial.println("temperature_too_high");
}
void temperature_in_range(float humidity,float temperature){
  Serial.print(humidity);
  Serial.print(" ");
  Serial.print(temperature);
  Serial.print(" ");
  Serial.println("temperature_in_range");
}