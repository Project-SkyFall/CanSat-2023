import serial
import time
from time import sleep

ser = serial.Serial(
        port='/dev/ttyAMA1',            #Seriový port ze ktérého čteme data
        baudrate = 9600,                #Rychlost vyčítání
        parity=serial.PARITY_NONE,      #paritní bit
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,      #Počet bitu potřebných k přečtení
        timeout=1                       #Počet potřebných zprav k timeoutu
        )

msg = ""                                #deklarace promenne
utc = ""
lat = ""
latd = ""
lon = ""
lond = ""
dataword = ""
        
def gps():
        global msg                              
        global utc
        global lat
        global latd
        global lon
        global lond
        gps_data=ser.readline()         #ctení ze seriálu
        #print (gps_data)
        sleep(0.15)
        msg_code = "$GPGGA"                #kod zprávy kterou hledáme
        if msg_code.encode() in gps_data:  #zjistujeme zda zpráva obsahuje náš kod
                msg = gps_data.decode()       #pokud ano ulozime zpravu do promenne
                ser.flushInput()
        #print(msg)
        if msg != "":
                word = msg.split(",")
                if int(word[7]) > 1 and word[2] != "" and word[4] != "":
                        lat = float(word[2][0:2]) + (float(word[2][2:11]) / 60.0)
                        latd = word[3]
                        lon = float(word[4][0:3]) + (float(word[4][3:12]) / 60.0)
                        lond = word[5]
                        utc = word[1][0:2] + ":" + word[1][2:4] + ":" + word[1][4:6]
                        
def DataWord():
        global utc
        global lat
        global latd
        global lon
        global lond
        global dataword

        latword = ("%0.8s" % lat)
        lonword = ("%0.8s" % lon)
        
        dataword = ("")
        #dataword = ("GPS; ")
        dataword += ("%s; " % utc)
        dataword += ("%s; " % latword)
        dataword += ("%s; " % latd)
        dataword += ("%s; " % lonword)
        dataword += ("%s; " % lond)

        print(dataword)

        with open("/home/pi/Desktop/gps.txt", "w") as file:
                file.write(dataword)
        
        
        
        

while True:
        gps()
        DataWord()
        time.sleep(0.1)
