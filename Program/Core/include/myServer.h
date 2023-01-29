#ifndef MYSERVER_H
#define MYSERVER_H

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
/*#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>*/

extern char ssid[];
extern char password[];

class MyWiFi : public WiFiClass{
    public:
    bool setup(char *ssid, char *password, bool verbose = false);
};

extern MyWiFi wifi;

class MyServer : public WebServer{//AsyncWebServer{
    public:
    MyServer(int port){};
    bool setup(bool verbose);
};

extern MyServer server;

void initialRequest();
void preFlightSettings();

#endif