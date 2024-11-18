#pragma once
class RELAY_MODULE
{
public:
    int pin;
    int state;
    RELAY_MODULE(int);
    ~RELAY_MODULE();
    void setup();
    void on();
    void off();
    void toggle();
};
