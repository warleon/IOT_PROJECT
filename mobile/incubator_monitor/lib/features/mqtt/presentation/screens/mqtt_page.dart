import 'package:flutter/material.dart';
import '../../domain/repositories/mqtt_repository.dart';
import '../../infrastructure/datasources/mqtt_datasource_impl.dart';
import '../../infrastructure/repositories/mqtt_repository_impl.dart';

class MQTTPage extends StatefulWidget {
  const MQTTPage({super.key});

  @override
  MQTTPageState createState() => MQTTPageState();
}

class MQTTPageState extends State<MQTTPage> {
  late final MQTTRepository _mqttRepository;
  String lastHumidity = 'No Data Available... Yet...';
  String lastTemperature = 'No Data Available... Yet...';
  String lastError = 'No Errors have been reported';
  final TextEditingController _servoController = TextEditingController();

  @override
  void initState() {
    super.initState();
    _initializeMQTT();
  }

  Future<void> _initializeMQTT() async {
    final dataSource = MQTTDatasourceImpl();
    _mqttRepository = MQTTRepositoryImpl(dataSource);

    // Conectar al broker
    await _mqttRepository.connect(
      broker: '192.168.18.9',
      clientIdentifier: 'flutter_client',
    );

    // Suscribir tópicos con sus respectivos callbacks
    _mqttRepository.subscribe('Incubator/humidity', (message) {
      setState(() => lastHumidity = message);
    });

    _mqttRepository.subscribe('Incubator/temperature', (message) {
      setState(() => lastTemperature = message);
    });

    _mqttRepository.subscribe('Incubator/error', (message) {
      setState(() => lastError = message);
    });
  }

  void _sendCommand(String command) {
    _mqttRepository.publish('Incubator/command', command);
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text('Incubator Monitor')),
      body: SingleChildScrollView(
        child: Padding(
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
                softWrap: true,
                overflow: TextOverflow.visible,
              ),
              const Text(
                'Temperature',
                style: TextStyle(fontSize: 22, fontWeight: FontWeight.bold),
              ),
              Text(
                lastTemperature,
                style: const TextStyle(fontSize: 18),
                softWrap: true,
                overflow: TextOverflow.visible,
              ),
              const Text(
                'Error',
                style: TextStyle(fontSize: 22, fontWeight: FontWeight.bold),
              ),
              Text(
                lastError,
                style: const TextStyle(fontSize: 18),
              ),
              const Text(
                'Controls',
                style: TextStyle(fontSize: 22, fontWeight: FontWeight.bold),
              ),
              const SizedBox(height: 10),
              Row(
                mainAxisAlignment: MainAxisAlignment.center,
                children: [
                  ElevatedButton(
                    onPressed: () => _sendCommand('light on'),
                    child: const Text('Light ON'),
                  ),
                  const SizedBox(width: 10),
                  ElevatedButton(
                    onPressed: () => _sendCommand('light off'),
                    child: const Text('Light OFF'),
                  ),
                ],
              ),
              const SizedBox(height: 10),
              Row(
                mainAxisAlignment: MainAxisAlignment.center,
                children: [
                  ElevatedButton(
                    onPressed: () => _sendCommand('fan on'),
                    child: const Text('Fan ON'),
                  ),
                  const SizedBox(width: 10),
                  ElevatedButton(
                    onPressed: () => _sendCommand('fan off'),
                    child: const Text('Fan OFF'),
                  ),
                ],
              ),
              const SizedBox(height: 20),
              Row(
                children: [
                  Expanded(
                    child: TextField(
                      controller: _servoController,
                      decoration: const InputDecoration(
                        border: OutlineInputBorder(),
                        labelText: 'Enter Servo Angle',
                      ),
                      keyboardType: TextInputType.number,
                    ),
                  ),
                  const SizedBox(width: 10),
                  ElevatedButton(
                    onPressed: () {
                      final angle = _servoController.text.trim();
                      if (angle.isNotEmpty && int.tryParse(angle) != null) {
                        _sendCommand('servo $angle');
                        _servoController.clear();
                      }
                    },
                    child: const Text('Move Servo'),
                  ),
                ],
              ),
            ],
          ),
        ),
      ),
    );
  }
}
