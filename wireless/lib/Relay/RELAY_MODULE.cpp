#include <RELAY_MODULE.hpp>
#include <Arduino.h>
RELAY_MODULE::RELAY_MODULE(int p):pin(p),state(HIGH)
{
}

RELAY_MODULE::~RELAY_MODULE()
{
}

void RELAY_MODULE::setup()
{
    pinMode(pin,OUTPUT);
    digitalWrite(pin,state);
}

void RELAY_MODULE::on()
{
    state=LOW;
    digitalWrite(pin,state);
}

void RELAY_MODULE::off()
{
    state=HIGH;
    digitalWrite(pin,state);
}

void RELAY_MODULE::toggle()
{
    state = !state;
    digitalWrite(pin,state);
}
