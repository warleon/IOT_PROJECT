#pragma once
#include <ESP32Servo.h>
#include <Time.hpp>

class SERVO_MODULE
{
private:
    static const int msPerDegree = 12;

    int pin;
    int angle;
    Servo servo;
    int currChange;
    Time timer;

    void clamp(int,int);
public:
    SERVO_MODULE(int);
    ~SERVO_MODULE();

    bool await();
    void set(int);
    void setup();
};
