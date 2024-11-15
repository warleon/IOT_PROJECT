#include <Time.hpp>
#include <Arduino.h>
Time::Time():current(millis()){}
void Time::tick(){
    last=current;
    current=millis();
    diff=current - last;
    acc+=diff;
}

void Time::reset()
{
    acc=0;
}
