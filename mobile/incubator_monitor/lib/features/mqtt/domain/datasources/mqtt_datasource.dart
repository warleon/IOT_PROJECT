abstract class MQTTDatasource {
  Future<void> connect(
      {required String broker, required String clientIdentifier});
  Future<void> subscribe(String topic, void Function(String message) callback);
  Future<void> publish(String topic, String message);
  Future<void> disconnect();
}
