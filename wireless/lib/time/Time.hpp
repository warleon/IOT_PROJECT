#pragma once
struct Time
{
    unsigned long last=0;
    unsigned long current;
    unsigned long diff=0;
    unsigned long acc=0;
    Time();
    void tick();
    void reset();
};
