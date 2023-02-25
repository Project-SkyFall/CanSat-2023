#ifndef MYSERVER_H
#define MYSERVER_H

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

extern char ssid[];
extern char password[];

class MyWiFi : public WiFiClass{
    public:
    MyWiFi(char *ssid, char *password);
    bool setup(bool verbose = false);

    Status status;

    private:
    char *_ssid;
    char *_password;
};

extern MyWiFi wifi;

class MyServer : public WebServer{//AsyncWebServer{
    public:
    MyServer(int port){};
    bool setup(bool verbose=false);
    void mode(bool run);
    void printStatus();

};

extern MyServer server;

void initialRequest();
void preFlightSettings();

#endif