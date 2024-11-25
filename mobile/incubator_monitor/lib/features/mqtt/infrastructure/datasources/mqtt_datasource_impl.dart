import 'package:mqtt_client/mqtt_client.dart';
import 'package:mqtt_client/mqtt_server_client.dart';

import '../../domain/datasources/mqtt_datasource.dart';

class MQTTDatasourceImpl implements MQTTDatasource {
  late MqttServerClient _client;
  final Map<String, void Function(String message)> _callbacks = {};
  bool _isReconnecting = false;

  @override
  Future<void> connect({
    required String broker,
    required String clientIdentifier,
  }) async {
    _client = MqttServerClient(broker, clientIdentifier)
      ..port = 1883
      ..keepAlivePeriod = 20
      ..logging(on: true)
      ..onDisconnected = _onDisconnected;
    _client.onConnected = () => print('Connected');
    _client.onSubscribed = (topic) => print('Subscribed to $topic');

    final connMessage = MqttConnectMessage()
        .withClientIdentifier(clientIdentifier)
        .startClean();
    _client.connectionMessage = connMessage;

    await _tryConnect();
  }

  Future<void> _tryConnect() async {
    try {
      print('Attempting to connect to broker...');
      await _client.connect();
      if (_client.connectionStatus?.state == MqttConnectionState.connected) {
        print('Connected to broker');
        _isReconnecting = false; // Reset reconnection flag
      } else {
        print('Failed to connect. Status: ${_client.connectionStatus}');
      }
    } catch (e) {
      print('Connection failed: $e');
      _client.disconnect();
      await _handleReconnect(); // Attempt reconnection
    }

    _client.updates?.listen((List<MqttReceivedMessage<MqttMessage?>> messages) {
      for (var message in messages) {
        final recMess = message.payload as MqttPublishMessage;
        final String topic = message.topic;
        final String payload =
            MqttPublishPayload.bytesToStringAsString(recMess.payload.message);

        if (_callbacks.containsKey(topic)) {
          _callbacks[topic]!(payload);
        }
      }
    });
  }

  Future<void> _handleReconnect() async {
    if (_isReconnecting) return; // Avoid concurrent reconnection attempts
    _isReconnecting = true;

    while (_client.connectionStatus?.state != MqttConnectionState.connected) {
      print('Reconnecting in 5 seconds...');
      await Future.delayed(const Duration(seconds: 5));
      await _tryConnect();
    }
  }

  void _onDisconnected() {
    print('Disconnected from broker');
    _handleReconnect(); // Attempt to reconnect automatically
  }

  @override
  Future<void> subscribe(
    String topic,
    void Function(String message) callback,
  ) async {
    _callbacks[topic] = callback;
    _client.subscribe(topic, MqttQos.atMostOnce);
  }

  @override
  Future<void> publish(String topic, String message) async {
    final builder = MqttClientPayloadBuilder()..addString(message);
    print('Publishing to $topic: $message');
    _client.publishMessage(topic, MqttQos.atLeastOnce, builder.payload!);
  }

  @override
  Future<void> disconnect() async {
    _client.disconnect();
    print('Disconnected from broker');
  }
}
