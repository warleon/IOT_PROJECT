#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

class DHT
{
private:
    DHT_Unified module;
    sensor_t humidity;
    sensor_t temperature;
    int delayMS;
public:
    DHT(int);
    void setup();
    void loop();
    ~DHT();
};