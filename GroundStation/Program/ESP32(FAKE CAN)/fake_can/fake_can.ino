/*********
  Modified from the examples of the Arduino LoRa library
  More resources: https://randomnerdtutorials.com
*********/

#include <SPI.h>
#include <LoRa.h>

//define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2

int counter = 0;

String datas[8] = {"cw_data1;1;325.00;1;25.50;997.89;42.17;18;1;2748;25;0.000000;0.000000;0.00;0.00;0;0;1;115.72;-24.42;-73.11;1;20.84;00006305","cw_data2;1;25.51;997.89;42.12;1;2.58;11.62;22.61;16.57;20.21;22.90;17.75;24.39;64.15;33.34;31.52;5.62;6.79;5.18;6.50;7.87;3.16;1.87;1;800;0;00007698","cw_data1;1;317.50;1;25.51;997.87;42.13;18;1;2734;25;0.000000;0.000000;0.00;0.00;0;0;1;115.69;-24.45;-72.82;1;20.85;00006318","cw_data2;1;25.52;997.89;42.13;1;2.58;11.62;22.61;16.57;20.21;22.90;17.75;24.89;64.15;32.88;31.52;5.62;6.79;4.32;6.50;7.87;3.16;1.87;1;800;0;00007708","cw_data1;1;315.00;1;25.52;997.88;42.09;17;1;2730;25;0.000000;0.000000;0.00;0.00;0;0;1;115.77;-24.47;-72.63;1;20.86;00006314","cw_data2;1;25.54;997.89;42.05;1;2.58;11.62;22.61;16.57;20.21;22.90;17.75;24.39;64.15;33.34;31.52;5.62;6.79;5.18;6.50;7.87;3.16;1.87;1;800;0;00007703","cw_data1;1;315.83;1;25.55;997.87;41.99;16;1;2719;25;0.000000;0.000000;0.00;0.00;0;0;1;115.71;-24.44;-73.33;1;20.86;00006330","cw_data2;1;25.55;997.92;41.99;1;2.58;11.62;22.61;16.57;20.21;22.90;17.75;24.39;64.15;33.34;31.52;5.62;6.79;4.32;6.50;7.87;3.16;1.87;1;800;0;00007705"};

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Sender");

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  
  //replace the LoRa.begin(---E-) argument with your location's frequency 
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(433E6)) {
    Serial.println(".");
    delay(500);
  }
   // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xFF);
  Serial.println("LoRa Initializing OK!");
}

void loop() {
  if (counter == 8) {
    counter = 0;
    }
  Serial.println(counter);
  LoRa.beginPacket();
  LoRa.print(datas[counter]);
  LoRa.endPacket();
  counter++;
  delay(200);
  Serial.println(counter);
  LoRa.beginPacket();
  LoRa.print(datas[counter]);
  LoRa.endPacket();
  counter++;
  delay(800);
  
 }
