#ifndef __E32_LOAD_NET__
#define __E32_LOAD_NET__
#include <WiFi.h>
#include <WebServer.h>
#include <AutoConnect.h>

#define VERSION 20210723


extern String mac;
void setupNetwork();
String getIp();
void portalLoop();

#endif
