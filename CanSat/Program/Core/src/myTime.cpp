#include "globalVars.h"

#include "myTime.h"

bool MyTime::setup(bool verbose){
    verbose ? Serial.println("---RTC setup--------------------------------------") : 0;

    getData();
    status = Status::status_OK;
    return true;
}

void MyTime::getData(){
    /*day = 26;
    month = 1;
    year = 2023;

    hour = 19;*/
    second++;
    if(second >= 60){
        second = 0;
        minute++;
    }
    

    date_string = ((String)day + "." + (String)month + "." + (String)year);
    dateTime_string = ((String)day + "." + (String)month + "." + (String)year + "-" + (String)hour + ":" + (String)minute + ":" + (String)second);
}

void MyTime::printData(){
    Serial.print("RTC: ");
    if(status == Status::status_SLEEP){
        Serial.println("SLEEPING");
        return;
    }
    else if(status == Status::status_FAIL){
        Serial.println("FAILED");
        return;
    }
    Serial.println(dateTime_string);
}