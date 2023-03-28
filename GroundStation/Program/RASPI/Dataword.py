import time
import binascii

adsbmeread = ("")
gpsread = ("")
loraread = ("")

Bit = [0,0,0,0,0,0,0]
CANAsx = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]

BHok = 0
BMEok = 0
SCDok = 0
MPUok = 0
OXYok = 0
ASXok = 0
SDok = 0

CANLightIntensity = 0
CANTemperature = 0
CANPressure = 0
CANHumidity = 0
CANBattery = 0
CANCo2 = 0
CANGPSstatus = 0
CANLatitude = 0
CANLongitude = 0
CANGPSAltitude = 0
CANGPSSpeed = 0
CANGPSTime = 0
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

    global CANLightIntesity
    global CANTemperature
    global CANPressure
    global CANHumidity
    global CANBattery
    global CANCo2
    global CANGPSstatus
    global CANLatitude
    global CANLongitude
    global CANGPSAltitude
    global CANGPSSpeed
    global CANGPSTime
    global CANGPSSattelites
    global CANRoll
    global CANPitch
    global CANYaw
    global CANOxygen
    global CANRefreshrate
    global CANAntena
    

    Data = loraread.split(";")
    
    try:
        dORa = int(Data[0], 16)
        #hexnum = ("47")
        #dORa = int(hexnum, 16)
    
        for i in range(7):
            Bit[i] = dORa & 0b0000001
            #print ("Bit číslo [%i]:" % i)
            #print (Bit[i])
            dORa = dORa >> 1
        
        BHok = Bit[0]
        BMEok = Bit[1]
        SCDok = Bit[2]
        MPUok = Bit[3]
        OXYok = Bit[4]
        ASXok = Bit[5]
        SDok = Bit[6]

        CANLightIntensity = Data[1]
        CANTemperature = Data[2]
        CANPressure = Data[3]
        CANHumidity = Data[4]
        CANBattery = Data[5]
        CANCo2 = Data[6]
        CANGPSstatus = Data[7]
        CANLatitude = Data[8]
        CANLongitude = Data[9]
        CANGPSAltitude = Data[10]
        CANGPSSpeed = Data[11]
        CANGPSTime = Data[12]
        CANGPSSattelites = Data[13]
        CANRoll = Data[14]
        CANPitch =  Data[15]
        CANYaw = Data[16]
        CANOxygen = Data[17]

        for j in range(18):
            CANAsx[j] = Data[18 + j]

        CANRefreshrate = Data[36]
        CANAntena = Data[37]    
    except:
        print("nodata in LORA file")
def AdsBmeparsdata():
    global adsbmeread
    global GSVoltage
    global GSTemperature
    global GSHumidity
    global GSPressure
    
    Data = adsbmeread.split(";")
    try:
        GSVoltage = Data[0]
        GSTemperature = Data[1]
        GSHumidity = Data[2]
        GSPressure = Data[3]
    except:
        print("nodata in BMEADS file")
def GPSparsdata():
    global gpsread
    global GSLatitude
    global GSLongitude
    global GSGPSTime

    Data = gpsread.split(";")

    try:
        GSLatitude = Data[0]
        GSLongitude = Data[1]
        GSGPSTime = Data[2]
    except:
        print("nodata in GPS file")
def DataWord():
    global BHok
    global BMEok
    global SCDok
    global MPUok
    global OXYok
    global ASXok
    global SDok

    global CANLightIntesity
    global CANTemperature
    global CANPressure
    global CANHumidity
    global CANBattery
    global CANCo2
    global CANGPSstatus
    global CANLatitude
    global CANLongitude
    global CANGPSAltitude
    global CANGPSSpeed
    global CANGPSTime
    global CANGPSSattelites
    global CANRoll
    global CANPitch
    global CANYaw
    global CANOxygen
    global CANRefreshrate
    global CANAntena

    global GSVoltage
    global GSTemperature
    global GSHumidity
    global GSPressure

    global GSLatitude
    global GSLongitude
    global GSGPSTime

    GSBattery = GSVoltage # doplnit výpočet

    GSCANRange = "1000" # doplnit

    GSAltitude = "220" # doplnit
    
    dataword = ("")
    dataword += ("%s;" % GSBattery)
    dataword += ("%s;" % GSTemperature)
    dataword += ("%s;" % GSHumidity)
    dataword += ("%s;" % GSPressure)
    dataword += ("%s;" % GSAltitude)
    dataword += ("%s;" % GSGPSTime)
    dataword += ("%s;" % GSLatitude)
    dataword += ("%s;" % GSLongitude)
    dataword += ("%s;" % GSCANRange)

    dataword += ("%s;" % BHok)
    dataword += ("%s;" % BMEok)
    dataword += ("%s;" % SCDok)
    dataword += ("%s;" % MPUok)
    dataword += ("%s;" % OXYok)
    dataword += ("%s;" % ASXok)
    dataword += ("%s;" % SDok)

    dataword += ("%s;" % CANLightIntensity)
    dataword += ("%s;" % CANTemperature)
    dataword += ("%s;" % CANPressure)
    dataword += ("%s;" % CANHumidity)
    dataword += ("%s;" % CANBattery)
    dataword += ("%s;" % CANCo2)
    dataword += ("%s;" % CANGPSstatus)
    dataword += ("%s;" % CANLatitude)
    dataword += ("%s;" % CANLongitude)
    dataword += ("%s;" % CANGPSAltitude)
    dataword += ("%s;" % CANGPSSpeed)
    dataword += ("%s;" % CANGPSTime)
    dataword += ("%s;" % CANGPSSattelites)
    dataword += ("%s;" % CANRoll)
    dataword += ("%s;" % CANPitch)
    dataword += ("%s;" % CANYaw)
    dataword += ("%s;" % CANOxygen)

    for k in range(18):
        dataword += ("%s;" % CANAsx[k])
    
    dataword += ("%s;" % CANRefreshrate)
    dataword += ("%s;" % CANAntena)
        
    with open("/home/pi/Desktop/Data.txt", "w") as file3:
        file3.write("%s" % dataword)
        
    print(dataword)
    
while True:
    ADSBMEread()
    AdsBmeparsdata()
    GPSread()
    GPSparsdata()
    LORAread()
    Loraparsdata()
    DataWord()
    time.sleep(0.5)
