import socket

def get_local_ip():
    try:
        # Crea un socket temporal para determinar la IP local
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        # Conecta a una dirección IP pública (no se envía tráfico real)
        s.connect(("8.8.8.8", 80))
        local_ip = s.getsockname()[0]
        s.close()
        return local_ip
    except Exception as e:
        print(f"Error obteniendo la IP local: {e}")
        return "127.0.0.1"  # Devuelve localhost si falla

# Obtén la IP automáticamente
MQTT_SERVER = get_local_ip()
print(f"Usando IP local: {MQTT_SERVER}")
