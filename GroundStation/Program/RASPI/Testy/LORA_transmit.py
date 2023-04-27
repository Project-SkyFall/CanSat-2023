import time
import busio
from digitalio import DigitalInOut
import board
import adafruit_rfm9x

# Configure LoRa Radio
CS = DigitalInOut(board.CE1)
RESET = DigitalInOut(board.D25)
spi = busio.SPI(board.SCK, MOSI=board.MOSI, MISO=board.MISO)
rfm9x = adafruit_rfm9x.RFM9x(spi, CS, RESET, 433.0)
#rfm9x.tx_power = 23
prev_packet = ""                              #Reset main storing variable
packet_text = ""
checked_packet = ""
final = ""
vysledek = None

soucet = 0
vycet = 1
ck = ""
ckedit = 0

sendpacket = "1"

rfm9x._write_u8(0xB9, 0xFF)

def commfromfile():
    global packet_text
    global vysledek
    global checked_packet
    
    line = filecommfile.readline()
    prev_packet = line.strip()
    #print(line)

    delka2 = (len(line))
    packet_text = line[12:delka2 - 3]
    vysledek = False
    ControlK()
    if vysledek == True:
        checked_packet = packet_text
        #print(checked_packet)
    else:
        print("bad ck")
    

def Dataword():
    global checked_packet
    global final
    #print(checked_packet)
    if checked_packet != "":
        delka = len(checked_packet)
        final = checked_packet[9:delka - 8]
        print(final)
        with open("/home/pi/Desktop/Lora.txt", "w") as lorafile:
            lorafile.write("%s" % final)
        final = ""
        checked_packet = ""

def ControlK():
    global soucet
    global vycet
    global ck
    global ckedit
    global packet_text
    global vysledek
    
    
    delka = (len(packet_text))
    a = delka - 8
    b = delka - 8

    for znak in packet_text:
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

    

def LoraReceive():                              #Basic lora function
    global vysledek
    global packet_text
    global checked_packet
    global prev_packet                          #Definition of variables
    global filecomm
    global sendpacket
    packet = None                               #Variable for storing packets (set to NONE every loop)
    packet = rfm9x.receive(with_header=True)    #Listening for packets, Header - first 4 letters of message
    if packet == None:
        return
    LoraSend()
    prev_packet = packet
    #print(prev_packet)
    with open("/home/pi/Desktop/LoRaCharsReceived.txt", "a") as errorfile:
        errorfile.write("\n%s" % prev_packet)

    try:
        packet_text = str(prev_packet, "utf-8") #Decoding packet to an UTF-8 symbol package
    except:
        print("error during decoding")

        with open("/home/pi/Desktop/LoRaCharsReceived.txt", "a") as errorfile:
            errorfile.write("\nerror during decoding")
                
    finally:
        vysledek = False
        ControlK()
        if vysledek == True:
            checked_packet = packet_text
            print(checked_packet)
        else:
            print("bad ck")
        CommandREAD()
        Dataword()
            
def CommandREAD():
    global sendpacket
            
    with open("/home/pi/Desktop/Command.txt", "r") as commandfile:
        if commandfile.readable():
            commandread = commandfile.read()
    if commandread == "":
        Command = "Salty Manager nevyžaduje žádný příkaz"
    elif commandread == "Restart":
        restart()
        return
    else:
        Command = commandread
        
    with open("/home/pi/Desktop/Command.txt", "w") as file1:
        file1.write("")
        
    soucet = 0
    for char in Command:
        soucet += int(ord(char))

    strsoucet = str(soucet)

    CK = ""
    for i in range(8 - (len(strsoucet))):
        CK += "0"
    CK += strsoucet


    sendpacket = Command
    sendpacket += ";"
    sendpacket += CK
    
    #print(sendpacket)
def restart():
    Command = "Debílku restartuj se"

    soucet = 0
    for char in Command:
        soucet += int(ord(char))

    strsoucet = str(soucet)

    CK = ""
    for i in range(8 - (len(strsoucet))):
        CK += "0"
    CK += strsoucet


    sendpacket = Command
    sendpacket += ";"
    sendpacket += CK

    rfm9x.send(bytes(sendpacket, "UTF-8"))
    print("sended")

    with open("/home/pi/Desktop/Command.txt", "w") as file1:
        file1.write("")
def LoraSend():
    global sendpacket
    print(sendpacket)
    rfm9x.send(bytes(sendpacket, "UTF-8"), keep_listening=True)
    print("sended")
    
while True:
    LoraReceive()

