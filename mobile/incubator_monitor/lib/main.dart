import 'package:flutter/material.dart';

import 'features/mqtt/presentation/screens/mqtt_page.dart';

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
