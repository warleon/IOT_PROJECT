import paho.mqtt.client as mqtt
import time
import random

# Configuración del broker MQTT
MQTT_SERVER = "192.168.18.9"  # Cambia por la IP de tu broker
MQTT_PORT = 1883
TOPICS = {
    "humidity": "Incubator/humidity",
    "temperature": "Incubator/temperature",
    "command": "Incubator/command",
    "error": "Incubator/error"
}

# Callback cuando se conecta al broker
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Conectado al broker MQTT")
    else:
        print(f"Error de conexión. Código: {rc}")

# Crear cliente MQTT (con especificación de versión API)
client = mqtt.Client(client_id="TestPublisher", protocol=mqtt.MQTTv311)

# Asignar callbacks
client.on_connect = on_connect

# Conectarse al broker
client.connect(MQTT_SERVER, MQTT_PORT, 60)

# Función para enviar datos simulados
def publish_mock_data():
    try:
        while True:
            # Simular datos de temperatura y humedad
            simulated_humidity = round(random.uniform(30.0, 70.0), 2)
            simulated_temperature = round(random.uniform(20.0, 35.0), 2)

            # Publicar datos en los tópicos correspondientes
            client.publish(TOPICS["humidity"], simulated_humidity)
            print(f"Publicado - Humedad: {simulated_humidity}")

            client.publish(TOPICS["temperature"], simulated_temperature)
            print(f"Publicado - Temperatura: {simulated_temperature}")

            # Pausa antes de enviar el próximo set de datos
            time.sleep(5)  # Envía datos cada 5 segundos
    except KeyboardInterrupt:
        print("Simulación detenida")
        client.disconnect()

# Función para enviar comandos de prueba
def send_test_command(command):
    client.publish(TOPICS["command"], command)
    print(f"Comando enviado: {command}")

# Ejecución
if __name__ == "__main__":
    client.loop_start()  # Inicia la escucha para mantener la conexión
    publish_mock_data()
