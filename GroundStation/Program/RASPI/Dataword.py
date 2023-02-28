import time

adsbmeread = ("")
gpsread = ("")
loraread = ("")

def LORAread():
    global loraread

    with open("/home/pi/Desktop/lora.txt", "r") as file0:
        if file0.readable():
            loraread = file0.read()
    print(loraread)

def ADSBMEread():
    global adsbmeread
    
    with open("/home/pi/Desktop/adsbme.txt", "r") as file1:
        if file1.readable():
            adsbmeread = file1.read()
    print(adsbmeread)

def GPSread():
    global gpsread
    
    with open("/home/pi/Desktop/gps.txt", "r") as file2:
        if file2.readable():
            gpsread = file2.read()
    print(gpsread)

def DataWord():
    dataword = ("")
    dataword += (adsbmeread)
    dataword += (gpsread)
    dataword += (loraread)

    with open("/home/pi/Desktop/Data.txt", "a") as file3:
        file3.write("\n%s" % dataword)
        

    print(dataword)
    
while True:
    ADSBMEread()
    GPSread()
    LORAread()
    DataWord()
    time.sleep(1)
