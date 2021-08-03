#include "mqtt.h"
#include "scale.h"
#include <ArduinoJson.h>

// C.F.,:
// https://github.com/marvinroger/async-mqtt-client/blob/develop/examples/FullyFeatured-ESP32/FullyFeatured-ESP32.ino
AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;

String outTopic;         // for publishing weight data
String inCalfactorTopic; // for setting scale calibration factor
String inTareTopic;      // for resetting scale to zero
String inResetTopic;     // for resetting ESP

void connectToMqtt()
{
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void onMqttConnect(bool sessionPresent)
{
  Serial.println("MQTT connected!");
  char buf[128];
  sprintf(buf, "{\"ip\": \"%s\", \"mac\": \"%s\", \"tpe\": \"start\", \"version\": \"%d\"}", getIp().c_str(), mac.c_str(), VERSION);

  // Publish device info
  mqttClient.publish("scale", 0, false, buf);

  outTopic = String("scale/") + mac;

  // Subscribe set scale calibration factor topic
  inCalfactorTopic = String("s2d/scale/") + mac + "/calfactor";
  mqttClient.subscribe(inCalfactorTopic.c_str(), 1);

  // Subscribe scale tare topic
  inTareTopic = String("s2d/scale/") + mac + "/tare";
  mqttClient.subscribe(inTareTopic.c_str(), 1);

  inResetTopic = String("s2d/scale/") + mac + "/reset";
  mqttClient.subscribe(inResetTopic.c_str(), 1);

  publisthMqtt(buf);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
  Serial.println("Disconnected from MQTT.");

  if (WiFi.isConnected())
  {
    xTimerStart(mqttReconnectTimer, 0);
  }
}
void onMqttSubscribe(uint16_t packetId, uint8_t qos)
{
}

void onMqttUnsubscribe(uint16_t packetId)
{
}

void onMqttMessage(char *topicBuf, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
  auto topic = String(topicBuf);

  if (topic == inTareTopic)
  {
    tare();
  }
  else if (topic == inCalfactorTopic)
  {
    StaticJsonDocument<256> data;
    // assuming no packet splitting here
    payload[len] = '\0';

    /* char new_payload[total + 1];
    new_payload[total] = '\0';
    strncpy(new_payload, payload, len); */
    
    // Serial.printf("Payload, index: %d, len: %d, total: %d, body: %s\r\n", index, len, total, new_payload);

    auto error = deserializeJson(data, payload);
    if (error)
    {
      String r("{\"error\": \"Parse json failed!\", \"tpe\": \"error\"}");
      publisthMqtt(r.c_str());
      return;
    }

    float val = data["val"].as<float>();
    bool persist = data["persist"].as<bool>();
    writeCalibrationFactor(val, persist);
  }
  else if (topic == inResetTopic)
  {
    ESP.restart();
  }
}

void onMqttPublish(uint16_t packetId)
{
  Serial.println("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void setupMqtt()
{
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);

  connectToMqtt();
}

void publisthMqtt(const char *payload)
{
  mqttClient.publish(outTopic.c_str(), 0, false, payload);
}
