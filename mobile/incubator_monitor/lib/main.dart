import 'package:flutter/material.dart';
import 'package:mqtt_client/mqtt_client.dart';
import 'mqttHandler.dart';

void main() {
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      title: 'MQTT Demo',
      theme: ThemeData(primarySwatch: Colors.blue),
      home: MQTTPage(),
    );
  }
}

class MQTTPage extends StatefulWidget {
  @override
  _MQTTPageState createState() => _MQTTPageState();
}

class _MQTTPageState extends State<MQTTPage> {
  final MQTTClientHandler _mqttHandler = MQTTClientHandler();
  String broker = '192.168.229.226'; // Example public broker
  String topic = 'Incubator/command';
  String receivedMessage = '';

  @override
  void initState() {
    super.initState();
    _connectToBroker();
  }

  Future<void> _connectToBroker() async {
    await _mqttHandler.connect(broker, 'flutter_client');
    _mqttHandler.subscribe(topic);

    // Listen for incoming messages
    _mqttHandler.client.updates!.listen((List<MqttReceivedMessage<MqttMessage?>> events) {
      final MqttPublishMessage recMess = events[0].payload as MqttPublishMessage;
      final String message =
      MqttPublishPayload.bytesToStringAsString(recMess.payload.message);
      setState(() {
        receivedMessage = message;
      });
      print('Received message: $message from topic: ${events[0].topic}>');
    });
  }

  void _sendMessage() {
    _mqttHandler.publish(topic, 'Hello from Flutter!');
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: Text('MQTT Demo')),
      body: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Text(
              'Received Message:',
              style: TextStyle(fontSize: 18),
            ),
            Text(
              receivedMessage,
              style: TextStyle(fontSize: 22, fontWeight: FontWeight.bold),
            ),
            SizedBox(height: 20),
            ElevatedButton(
              onPressed: _sendMessage,
              child: Text('Send Message'),
            ),
          ],
        ),
      ),
    );
  }
}
