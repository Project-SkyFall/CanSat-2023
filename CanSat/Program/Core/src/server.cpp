#include "globalVars.h"

char ssid[] = "Project-SkyFall";
char password[] = "1234abcd";

bool MyWiFi::setup(char *ssid, char *password, bool verbose){
    Serial.println("---WiFi setup---");
    softAP(ssid, password);
    IPAddress AP_LOCAL_IP(192, 168, 10, 10);
    IPAddress AP_GATEWAY_IP(192, 168, 10, 10);
    IPAddress AP_NETWORK_MASK(255, 255, 255, 0);
    softAPConfig(AP_LOCAL_IP, AP_GATEWAY_IP, AP_NETWORK_MASK);
    Serial.print("CanSat IP address: "); Serial.println(WiFi.softAPIP());
    return true;
}

char* defaultRoute = "/server/";

bool MyServer::setup(bool verbose){
    verbose ? Serial.println("--Server setup--") : 0;
    on("/", initialRequest);
    //MyServer::on("/pre-flight-settings", preFlightSettings);
    serveStatic("/", SD, defaultRoute);
    //MyServer::serveStatic("/", SD, defaultRoute);
    begin();
    return true;
};

/*void serverSetup(){
    server.on("/", initialRequest);
    server.on("/pre-flight-settings", preFlightSettings);
    server.serveStatic("/", SD, defaultRoute);
}*/

void sdOpenSend(String path){
    String output;
    myFile = SD.open(defaultRoute + path);
    if(myFile){
        if(myFile.size() == 0){
            Serial.println("No content");
            server.send(204, "text/html", "No content");
            return;
        }
        while(myFile.available()){
            output += (char)myFile.read();
        }
        Serial.println("Data loaded OK");
        server.send(200, "text/html", output);
        return;
    }
    Serial.println("File not found");
    server.send(404, "text/html", "File not found");
}

void initialRequest(){
    if(server.method() == HTTP_GET){
        sdOpenSend("index.htm");
        //server.send(200, "text/plain", "OK");
        return;
    }
    server.send(405, "text/plain", "Bad method");
}

/*void preFlightSettings(){
    if(server.method() == HTTP_GET){
        sdOpenSend("/preFlightSettings.html");
        return;
    }
    server.send(405, "html/text", "Bad method");
    
}*/