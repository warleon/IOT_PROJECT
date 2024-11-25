import '../../domain/datasources/mqtt_datasource.dart';
import '../../domain/repositories/mqtt_repository.dart';

class MQTTRepositoryImpl implements MQTTRepository {
  final MQTTDatasource _dataSource;

  MQTTRepositoryImpl(this._dataSource);

  @override
  Future<void> connect({
    required String broker,
    required String clientIdentifier,
  }) async {
    await _dataSource.connect(
      broker: broker,
      clientIdentifier: clientIdentifier,
    );
  }

  @override
  Future<void> subscribe(
    String topic,
    void Function(String message) callback,
  ) async {
    await _dataSource.subscribe(topic, callback);
  }

  @override
  Future<void> publish(String topic, String message) async {
    await _dataSource.publish(topic, message);
  }

  @override
  Future<void> disconnect() async {
    await _dataSource.disconnect();
  }
}
