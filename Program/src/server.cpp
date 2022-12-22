/*#include "server.h"
#include "mySD.h"

char* defaultRoute = "source/web";

void serverSetup(){
    server.on("/", initialRequest);
    server.on("/pre-flight-settings", preFlightSettings);
    server.serveStatic("/", SD, defaultRoute);
}

void sdOpenSend(String path){
    String output;
    if(SD.open(defaultRoute + path)){
        if(myFile.size() == 0){
            server.send(204, "html/text", "No content");
            return;
        }
        while(myFile.available()){
            output += (char)myFile.read();
        }
        server.send(200, "html/text", output);
        return;
    }
    server.send(404, "html/text", "File not found");
}

void initialRequest(){
    if(server.method() == HTTP_GET){
        sdOpenSend("/index.html");
        return;
    }
    server.send(405, "html/text", "Bad method");
}

void preFlightSettings(){
    if(server.method() == HTTP_GET){
        sdOpenSend("/preFlightSettings.html");
        return;
    }
    server.send(405, "html/text", "Bad method");
    
}*/