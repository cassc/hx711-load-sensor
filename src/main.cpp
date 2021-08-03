#include <Arduino.h>
#include <HX711.h>
#include "base.h"
#include "mqtt.h"
#include "scale.h"

String mac;

char mqMsgBuf[128];


void setup()
{
	Serial.begin(115200);

	setupNetwork();

	setupMqtt();

	setupScale();
}

unsigned long prevTs = 0;

void loop()
{
	portalLoop();

	if (millis() - prevTs > 2000)
	{
		prevTs = millis();
		sprintf(mqMsgBuf, "{\"weight\": %.3f, \"tpe\": \"weight\"}", getWeight());
		publisthMqtt(mqMsgBuf);
	}
}
