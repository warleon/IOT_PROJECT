# DOCS

- [ESP32 user guide](https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32/esp32-devkitc/user_guide.html#overview)

- [DHT11 sensor info](https://www.electronicwings.com/sensors-modules/dht11)

```mermaid
flowchart TD
    Start --> SetupBegin
    SetupBegin --> wificonfig[Configures WiFi & client] --> sensorconfig[Configures Sensor]  --> servoconfig[Configures Servo] -->fanconfig[Configures Fan] --> lightconfig[Configures light] --> SetupEnd --> Tick

    Tick[Loop Begin]
    Tick -->clienttick[Reads Messages]
    Tick -->sensortick[Reads Sensor]
    Tick --> |Every 5 minutes| servotick[Moves Servo]

    clienttick --> OnMessage
    OnMessage -->|Calls| OnCommand
    OnCommand -->|On Servo Command| servotick
    OnCommand -->|Controls| light
    OnCommand -->|Controls| fan

    sensortick --> OnSensorRead
    OnSensorRead --> |if temperature low| TempLow
    OnSensorRead --> |if temperature high| TempHigh
    OnSensorRead ---> |if temperature in range| TempRange
    OnSensorRead --> |if error| SensorError -->|Publish Error| client


    TempLow -->|Fan OFF| fan
    TempHigh -->|Fan ON| fan
    TempLow -->|Light ON| light
    TempHigh -->|Light OFF| light

    servotick & light & fan & TempRange & client --> LoopEnd

    LoopEnd[Loop End] --> Tick
```
