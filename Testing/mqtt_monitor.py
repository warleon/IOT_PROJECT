import paho.mqtt.client as mqtt

# Configuración del broker MQTT
MQTT_SERVER = "192.168.18.9"
MQTT_PORT = 1883
TOPICS = "Incubator/#"  # Suscríbete a todos los tópicos bajo 'Incubator/'

# Callback cuando se conecta al broker
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Conectado al broker MQTT")
        client.subscribe(TOPICS)  # Suscribirse al tópico
    else:
        print(f"Error de conexión. Código: {rc}")

# Callback cuando se recibe un mensaje
def on_message(client, userdata, msg):
    print(f"Tópico: {msg.topic}, Mensaje: {msg.payload.decode()}")

# Crear cliente MQTT
client = mqtt.Client(client_id="TestSubscriber", protocol=mqtt.MQTTv311)
client.on_connect = on_connect
client.on_message = on_message

# Conexión al broker
client.connect(MQTT_SERVER, MQTT_PORT, 60)

# Bucle para mantener la conexión
client.loop_forever()
