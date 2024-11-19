import 'package:mqtt_client/mqtt_client.dart';
import 'package:mqtt_client/mqtt_server_client.dart';

class MQTTClientHandler {
  late MqttServerClient client;

  Future<void> connect(String broker, String clientIdentifier) async {
    client = MqttServerClient(broker, clientIdentifier);

    // Configure client options
    client.port = 1883; // Default Mosquitto port
    client.keepAlivePeriod = 20;
    client.logging(on: true);
    client.onDisconnected = onDisconnected;
    client.onConnected = onConnected;
    client.onSubscribed = onSubscribed;

    // Optional: Add a last will message
    final connMessage = MqttConnectMessage()
        .withClientIdentifier(clientIdentifier)
        .withWillTopic('lastWillTopic')
        .withWillMessage('Client disconnected unexpectedly')
        .startClean(); // Clean session
    client.connectionMessage = connMessage;

    try {
      // Attempt to connect
      print('Connecting to MQTT broker...');
      await client.connect();
    } catch (e) {
      print('Connection failed: $e');
      client.disconnect();
    }

    if (client.connectionStatus?.state == MqttConnectionState.connected) {
      print('Connected to MQTT broker.');
    } else {
      print('Failed to connect. Status: ${client.connectionStatus}');
    }
  }

  void subscribe(String topic) {
    print('Subscribing to $topic...');
    client.subscribe(topic, MqttQos.atMostOnce);
  }

  void publish(String topic, String message) {
    final builder = MqttClientPayloadBuilder();
    builder.addString(message);
    print('Publishing to $topic: $message');
    client.publishMessage(topic, MqttQos.atLeastOnce, builder.payload!);
  }

  void onDisconnected() {
    print('MQTT Client Disconnected.');
  }

  void onConnected() {
    print('MQTT Client Connected.');
  }

  void onSubscribed(String topic) {
    print('Subscribed to topic: $topic');
  }

  void disconnect() {
    client.disconnect();
    print('Client disconnected.');
  }
}
