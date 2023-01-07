#include "myINA219.h"
#include "globalVars.h"

void printINA(){
    Serial.print(ina.getBusVoltage_V());
    Serial.print("    ");
    Serial.println(ina.getCurrent_mA());
}