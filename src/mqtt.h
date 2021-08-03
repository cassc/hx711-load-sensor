#ifndef __E32_LOAD_MQTT__
#define __E32_LOAD_MQTT__

extern "C" {
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
}
#include <WiFi.h>
#include <AsyncMqttClient.h>
#include "base.h"
#include "net.h"

#define MQTT_HOST IPAddress(10, 0, 2, 78)
#define MQTT_PORT 1883

void setupMqtt();
void publisthMqtt(const char* payload);


#endif
