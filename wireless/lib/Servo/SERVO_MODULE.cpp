#include "SERVO_MODULE.hpp"

void SERVO_MODULE::clamp(int min, int max)
{
    if(angle<min)angle=min;
    else if(angle > max) angle=max;
}

SERVO_MODULE::SERVO_MODULE(int p) : pin(p), angle(0),currChange(0)
{
}

SERVO_MODULE::~SERVO_MODULE()
{
}

bool SERVO_MODULE::await()
{
    timer.tick();
    if(timer.acc < msPerDegree * currChange)return false;
    currChange = 0;
    timer.reset();
    return true;
}

void SERVO_MODULE::set(int nangle)
{
    timer.tick();
    if(timer.acc < msPerDegree * currChange)return;//ignore command while moving 
    int curr = angle;
    angle = nangle;
    clamp(0,180);
    currChange= abs(angle -curr);
    servo.write(angle);
    timer.reset();
}

void SERVO_MODULE::setup()
{
    servo.attach(pin);
    servo.write(angle);
    delay(msPerDegree * 180);
    
}
