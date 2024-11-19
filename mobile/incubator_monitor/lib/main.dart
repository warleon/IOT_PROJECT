import 'dart:ffi';

import 'package:flutter/material.dart';
import 'package:mqtt_client/mqtt_client.dart';
import 'mqttHandler.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      title: 'Incubator Monitor',
      theme: ThemeData(primarySwatch: Colors.blue),
      home: const MQTTPage(),
    );
  }
}

class MQTTPage extends StatefulWidget {
  const MQTTPage({super.key});

  @override
  _MQTTPageState createState() => _MQTTPageState();
}

class _MQTTPageState extends State<MQTTPage> {
  final MQTTClientHandler _mqttHandler = MQTTClientHandler();
  final TextEditingController _messageController = TextEditingController();


  String broker = '192.168.229.226'; // Example public broker
  final String pubTopic = 'Incubator/command';
  final List<String> subTopics =['Incubator/humidity','Incubator/temperature','Incubator/error'];

  String lastHumidity='No Data Available... Yet...';
  String lastTemperature='No Data Available... Yet...';
  String lastError='No Errors have been reported';

  @override
  void initState() {
    super.initState();
    _connectToBroker();
  }

  void onMessage(String topic, String msg){
      if(topic == subTopics[0]){
        setState(() {
          lastHumidity = msg;
        });
      }
      else if(topic == subTopics[1]){
        setState(() {
          lastTemperature = msg;
        });
      }
      else if(topic == subTopics[2]){
        setState(() {
          lastError = msg;
        });
      }
  }

  Future<void> _connectToBroker() async {
    await _mqttHandler.connect(broker, 'flutter_client');
    for(String topic in subTopics){
      _mqttHandler.subscribe(topic);
    }

    // Listen for incoming messages
    _mqttHandler.client.updates!.listen((List<MqttReceivedMessage<MqttMessage?>> events) {
      final MqttPublishMessage recMess = events[0].payload as MqttPublishMessage;
      final String message =
      MqttPublishPayload.bytesToStringAsString(recMess.payload.message);
      final String topic = events[0].topic;

      print('Received message: $message from topic: ${topic}>');
      onMessage(topic, message);
    });
  }

  void _sendMessage() {
    final String message = _messageController.text.trim();
    if (message.isNotEmpty) {
      _mqttHandler.publish(pubTopic, message);
      _messageController.clear(); // Clear the text field after sending
    } else {
      print('Message is empty. Cannot send.');
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text('Incubator Monitor')),
      body: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            const Text(
              'Humidity',
              style: TextStyle(fontSize: 22, fontWeight: FontWeight.bold),
            ),
            Text(
              lastHumidity,
              style: const TextStyle(fontSize: 18),
            ),
            const Text(
              'Temperature',
              style: TextStyle(fontSize: 22, fontWeight: FontWeight.bold),
            ),
            Text(
              lastTemperature,
              style: const TextStyle(fontSize: 18),
            ),
            const Text(
              'Error',
              style: TextStyle(fontSize: 22, fontWeight: FontWeight.bold),
            ),
            Text(
              lastError,
              style: const TextStyle(fontSize: 18),
            ),
            const SizedBox(height: 20),
            TextField(
              controller: _messageController,
              decoration: const InputDecoration(
                border: OutlineInputBorder(),
                labelText: 'Enter your command',
              ),
            ),
            const SizedBox(height: 20),
            ElevatedButton(
              onPressed: _sendMessage,
              child: const Text('Send Message'),
            ),
          ],
        ),
      ),
    );
  }
}
