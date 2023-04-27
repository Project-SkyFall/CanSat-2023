import time
import board
import busio
import digitalio
import RPi.GPIO as io
import adafruit_rfm9x

soucet = 0
vycet = 1
ck = ""
ckedit = 0


def CK(word):
    global soucet
    global vycet
    global ck
    global ckedit
    
    delka = (len(word))
    a = delka - 8
    
    for znak in word:
        if a > 0:
            #print (znak)
            soucet += int((ord(znak)))
        if a < 0:
            #print (znak)
            ck += znak
        a = a - 1

    #print (soucet)
    #print (ck)
    try:
        ckedit = int(ck)
        
    except:
        print ("Cant convert CK")
        
    if ckedit == soucet:
        vysledek = True
        #print("true")
        soucet = 0
        ck = ""
    else:
        vysledek = False
        #print("false")
        soucet = 0
        ck = ""
    return(vysledek)
test = "ahoj;00000477"
while True:
    CK(test)
    #print(test)
    print(CK(test))
