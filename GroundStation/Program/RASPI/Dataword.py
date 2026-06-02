import time
import math

from datetime import datetime

import os
import os.path

adsbmeread = ("")
gpsread = ("")
loraread = ("")

Bit = [0,0,0,0,0,0,0,0,0,0,0,0]
CANAsx = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]

BHok = 0
BMEok = 0
SCDok = 0
MPUok = 0
OXYok = 0
ASXok = 0
SDok = 0
GPSok = 0
INAok = 0
NEOok = 0
DS18ok = 0
CAMok = 0

CANRTCtime = 0
CANLightIntensity = 0
CANTemperature = 0
CANPressure = 0
CANHumidity = 0
CANVoltage = 0
CANCurrent =0
CANPower = 0
CANCo2 = 0
CANLatitude = 0
CANLongitude = 0
CANGPSAltitude = 0
CANGPSSattelites = 0
CANRoll = 0
CANPitch =  0
CANYaw = 0
CANOxygen = 0

GSVoltage = 0
GSTemperature = 0
GSHumidity = 0
GSPressure = 0

GSLatitude = 0
GSLongitude = 0
GSGPSTime = 0

Cycle = 0
cas = int(time.time())
cycletime = cas
    
def Cycle():
    global cas
    global cycletime
    if cas + 1 < time.time():
        cycle = cas - cycletime
        #print(cycle)
        #print(int(time.time()))
        cas = int(time.time())
        #print(cycle)
        return int(cycle)
    
def LORAread():
    global loraread

    with open("/home/pi/Desktop/Lora.txt", "r") as file0:
        if file0.readable():
            loraread = file0.read()
    #print(loraread)

def ADSBMEread():
    global adsbmeread
    
    with open("/home/pi/Desktop/adsbme.txt", "r") as file1:
        if file1.readable():
            adsbmeread = file1.read()
    #print(adsbmeread)

def GPSread():
    global gpsread
    
    with open("/home/pi/Desktop/gps.txt", "r") as file2:
        if file2.readable():
            gpsread = file2.read()
    #print(gpsread)

def Loraparsdata():
    global loraread
    global BHok
    global BMEok
    global SCDok
    global MPUok
    global OXYok
    global ASXok
    global SDok
    global GPSok
    global INAok
    global NEOok
    global DS18ok
    global CAMok
    
    global CANRTCtime
    global CANLightIntesity
    global CANTemperature
    global CANPressure
    global CANHumidity
    global CANVoltage
    global CANCurrent
    global CANPower
    global CANCo2
    global CANGPSstatus
    global CANLatitude
    global CANLongitude
    global CANGPSAltitude
    global CANGPSSattelites
    global CANRoll
    global CANPitch
    global CANYaw
    global CANOxygen
    global CANRefreshrate

    
    if loraread == "":
        return
    
    Data = loraread.split(";")
    
    dORa = int(Data[1], 16)
    #hexnum = ("47")
    #dORa = int(hexnum, 16)
    
    for i in range(12):
        Bit[i] = dORa & 0b0000011
        if Bit[i] == 3:
            Bit[i] = 2
        dORa = dORa >> 2
        
    BHok = Bit[0]
    BMEok = Bit[1]
    SCDok = Bit[2]
    MPUok = Bit[3]
    OXYok = Bit[4]
    ASXok = Bit[5]
    SDok = Bit[6]
    GPSok = Bit[7]
    INAok = Bit[8]
    NEOok = Bit[9]
    DS18ok = Bit[10]
    CAMok = Bit[11]

    CANRTCtime = Data[2]
    CANLightIntensity = Data[3]
    CANTemperature = Data[4]
    CANPressure = Data[5]
    CANHumidity = Data[6]
    CANVoltage = Data[7]
    CANCurrent = Data[8]
    CANPower = Data[9]
    CANCo2 = Data[10]
    CANLatitude = Data[11]
    CANLongitude = Data[12]
    CANGPSAltitude = Data[13]
    CANGPSSattelites = Data[14]
    CANRoll = Data[15]
    CANPitch =  Data[16]
    CANYaw = Data[17]
    CANOxygen = Data[18]

    for j in range(18):
        CANAsx[j] = Data[19 + j]

    CANRefreshrate = Data[37]    

def AdsBmeparsdata():
    global adsbmeread
    global GSVoltage
    global GSTemperature
    global GSHumidity
    global GSPressure

    if adsbmeread == "":
        return
    
    Data = adsbmeread.split(";")
    
    GSVoltage = Data[0]
    GSTemperature = Data[1]
    GSHumidity = Data[2]
    GSPressure = Data[3]

def GPSparsdata():
    global gpsread
    global GSLatitude
    global GSLongitude
    global GSGPSTime

    if gpsread == "":
        return

    Data = gpsread.split(";")

    GSLatitude = Data[0]
    GSLongitude = Data[1]
    GSGPSTime = Data[2]

def DataWord():
    global BHok
    global BMEok
    global SCDok
    global MPUok
    global OXYok
    global ASXok
    global SDok
    global GPSok
    global INAok
    global NEOok
    global DS18ok
    global CAMok

    global cycle
    global CANRTCtime
    global CANLightIntesity
    global CANTemperature
    global CANPressure
    global CANHumidity
    global CANVoltage
    global CANCurrent
    global CANPower
    global CANCo2
    global CANLatitude
    global CANLongitude
    global CANGPSAltitude
    global CANGPSSattelites
    global CANRoll
    global CANPitch
    global CANYaw
    global CANOxygen
    global CANRefreshrate

    global GSVoltage
    global GSTemperature
    global GSHumidity
    global GSPressure

    global GSLatitude
    global GSLongitude
    global GSGPSTime

    
    try:
        Batteryvolt = float(GSVoltage)
    except:
        print("cant convert battery voltage")

    Batterylowvolt = 3.0
    Batteryhighvolt = 4.2
    
    GSBattery = ((Batteryvolt - Batterylowvolt) / (Batteryhighvolt - Batterylowvolt)) * 100 # doplnit výpočet

    if GSBattery <= 0:
        GSBattery = 0
    elif GSBattery >= 100:
        GSBattery = 100
        
    Sealevelpressure = 1021

    GPress = float(GSPressure) / Sealevelpressure
    GSAltitude = (math.log10(GPress) / math.log10(0.88)) * 1000
    
    CPress = float(CANPressure) / Sealevelpressure
    CANAltitude = (math.log10(CPress) / math.log10(0.88)) * 1000

    Height = CANAltitude - GSAltitude

    if Height <= 0:
        Height = 0
    
    Distance = math.sqrt(math.pow(float(GSLatitude) - float(CANLatitude), 2) + math.pow(float(GSLongitude) - float(CANLongitude), 2)) * 0.012

    dataword = ("")
    dataword += ("%s;" % GSBattery)
    dataword += ("%s;" % GSTemperature)
    dataword += ("%s;" % GSHumidity)
    dataword += ("%s;" % GSPressure)
    dataword += ("%s;" % GSAltitude)
    dataword += ("%s;" % GSGPSTime)
    dataword += ("%s;" % GSLatitude)
    dataword += ("%s;" % GSLongitude)
    dataword += ("%s;" % Distance)

    dataword += ("%s;" % BHok)
    dataword += ("%s;" % BMEok)
    dataword += ("%s;" % SCDok)
    dataword += ("%s;" % MPUok)
    dataword += ("%s;" % OXYok)
    dataword += ("%s;" % ASXok)
    dataword += ("%s;" % SDok)
    dataword += ("%s;" % GPSok)
    dataword += ("%s;" % INAok)
    dataword += ("%s;" % NEOok)
    dataword += ("%s;" % DS18ok)
    
    dataword += ("%s;" % CANRTCtime)
    dataword += ("%s;" % CANLightIntensity)
    dataword += ("%s;" % CANTemperature)

    CANPressurekPa = float(CANPressure) / 10
    
    dataword += ("%s;" % CANPressurekPa)
    dataword += ("%s;" % CANHumidity)
    dataword += ("%s;" % CANVoltage)
    dataword += ("%s;" % CANCurrent)
    dataword += ("%s;" % CANPower)
    dataword += ("%s;" % CANCo2)
    dataword += ("%s;" % CANLatitude)
    dataword += ("%s;" % CANLongitude)
    dataword += ("%s;" % CANAltitude)
    dataword += ("%s;" % Height)
    dataword += ("%s;" % CANGPSSattelites)
    dataword += ("%s;" % CANRoll)
    dataword += ("%s;" % CANPitch)
    dataword += ("%s;" % CANYaw)
    dataword += ("%s;" % CANOxygen)

    for k in range(18):
        dataword += ("%s;" % CANAsx[k])

    Frequency = 1000 / int(CANRefreshrate)
    
    dataword += ("%s;" % Frequency)
    dataword += ("%s;" % CAMok)
            
    dataword += ("%s;" % cycle)
        
    with open("/home/pi/Desktop/Data.txt", "w") as file3:
        file3.write("%s" % dataword)

    with open("/home/pi/Desktop/Backup-data.txt", "a") as file4:
        file4.write("%s\n" % dataword)
        
    print(dataword)

def backupfile():
    global datetime
    
    zahlavi = "batteryGS;tempGS;humidityGS;pressGS;altGS;timeGS;latGS;longGS;rangeCan;dORa4;dORa0;dORa3;dORa1;dORa2;dORa5;dORa7;dORa10;dORa8;dORa6;dORa9;time;lightIntensity;temperature;pressure;humidity;battery;current;power;co2;lat;long;height_GPS;height;sattelitesNum;roll;pitch;yaw;oxygen;asx0;asx1;asx2;asx3;asx4;asx5;asx6;asx7;asx8;asx9;asx10;asx11;asx12;asx13;asx14;asx15;asx16;asx17;transfer;dORa11;time2;"
    
    datetimenow = datetime.now()
    date = datetimenow.strftime("%d_%m_%Y_%H_%M")

    old_name = r"/home/pi/Desktop/Backup-data.txt"
    new_name  =r"/home/pi/Desktop/backups/Backup%s.txt" % date
    
    if not(os.path.exists("/home/pi/Desktop/Backup-data.txt")):
        with open("/home/pi/Desktop/Backup-data.txt", "w") as backupfile:
            backupfile.write("%s\n" % zahlavi)
    else:
        os.rename(old_name, new_name)
        with open("/home/pi/Desktop/Backup-data.txt", "w") as backupfile:
            backupfile.write("%s\n" % zahlavi)
    
backupfile()
while True:
    cycle = Cycle()
    if cycle != None:
        ADSBMEread()
        AdsBmeparsdata()
        GPSread()
        GPSparsdata()
        LORAread()
        Loraparsdata()
        DataWord()
    
