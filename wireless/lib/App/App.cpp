#include <App.hpp>
#include <sstream>
App* App::instance;

App::App(String name,periferals_info_t pinnes):name(name),sensor(pinnes.dht_pin,{40.0f,60.0f},{36.0f,39.0f}),servo(pinnes.servo_pin),fan(pinnes.fan_pin),light(pinnes.light_pin),client(wifi)
{
    instance = this;
    topics.humidity = name + "/humidity";
    topics.temperature = name + "/temperature";
    topics.error = name + "/error";
    topics.command = name + "/command";
}

void App::setup(config_t config)
{
    sensor.setup();
    servo.setup();
    fan.setup();
    light.setup();

    sensor.on[sensor.HUMIDITY_TOO_LOW] = humidity_too_low;
    sensor.on[sensor.HUMIDITY_TOO_HIGH] = humidity_too_high;
    sensor.on[sensor.HUMIDITY_IN_RANGE] = humidity_in_range;
    sensor.on[sensor.TEMPERATURE_TOO_LOW] = temperature_too_low;
    sensor.on[sensor.TEMPERATURE_TOO_HIGH] = temperature_too_high;
    sensor.on[sensor.TEMPERATURE_IN_RANGE] = temperature_in_range;
    sensor.on_error = sensor_error;


    WiFi.begin(config.SSID.c_str(),config.password.c_str());
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Conectando al WiFi...");
    }
    Serial.println("Conectado al WiFi");

    client.setServer(config.mqttServer.c_str(),config.mqttPort);
    while (!client.connected()) {
        Serial.println("Conectando al broker MQTT...");
        if (client.connect(name.c_str(), config.mqttUser.c_str(), config.mqttPassword.c_str())) {
            Serial.println("Conectado al broker MQTT");
        } else {
            Serial.print("Error de conexión: ");
            Serial.println(client.state());
            delay(2000);
        }
    }
    client.setCallback(on_message).subscribe(topics.command.c_str());
    servo.set(90);
}

void App::tick()
{
    timer.tick();
    client.loop();
    sensor.tick();
    if(timer.acc>5*60*1000){
        servo.set(90-15);
        while (!servo.await());//blocking
        servo.set(90+15);
        timer.reset();
    }
}

App::~App()
{
}

void App::on_sensor_read(float humidity, float temperature)
{
    auto msg = std::to_string(humidity);
    client.publish(topics.humidity.c_str(),msg.c_str());

    msg = std::to_string(temperature);
    client.publish(topics.temperature.c_str(),msg.c_str());
}

void App::on_command(std::string command)
{
    std::stringstream ss(command);
    std::string token;
    ss>>token;
    if(token=="servo"){
        int angle;
        ss>>angle;
        servo.set(angle);
    }
    else if(token=="light"){
        ss>>token;
        if(token=="on"){
            light.on();
        }
        else if(token=="off"){
            light.off();
        }
    }
    else if(token=="fan"){
        ss>>token;
        if(token=="on"){
            fan.on();
        }
        else if(token=="off"){
            fan.off();
        }
    }
}

void App::on_message(char * topic, uint8_t * msg, unsigned int size)
{
    if(!strcmp(topic,instance->topics.command.c_str())){
        instance->on_command(std::string(String(msg,size).c_str()));
    }
}

void App::humidity_too_low(float humidity,float temperature){
    instance->on_sensor_read(humidity,temperature);
    Serial.println("humidity_too_low");
}
void App::humidity_too_high(float humidity,float temperature){
    instance->on_sensor_read(humidity,temperature);
    Serial.println("humidity_too_high");
}
void App::humidity_in_range(float humidity,float temperature){
    instance->on_sensor_read(humidity,temperature);
    Serial.println("humidity_in_range");
}
void App::temperature_too_low(float humidity,float temperature){
    instance->fan.off();
    instance->light.on();
    instance->on_sensor_read(humidity,temperature);
    Serial.println("temperature_too_low");
}
void App::temperature_too_high(float humidity,float temperature){
    instance->light.off();
    instance->fan.on();
    instance->on_sensor_read(humidity,temperature);
    Serial.println("temperature_too_high");
}
void App::temperature_in_range(float humidity,float temperature){
    instance->on_sensor_read(humidity,temperature);
    Serial.println("temperature_in_range");
}

void App::sensor_error(String err)
{
    instance->client.publish(instance->topics.error.c_str(),err.c_str());
}
