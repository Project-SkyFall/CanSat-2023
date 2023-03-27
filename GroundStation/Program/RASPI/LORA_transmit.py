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

uartpacket = ""

rfm9x._write_u8(0xB9, 0xFF)

filecommfile = open('/home/pi/Desktop/filecomm.txt', 'r')

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
    

def uartcomm():
    global uartpacket
    global checked_packet
    global final
    vec = ""
    slovo = ""

    a = 0
    delka = (len(checked_packet))
    for znak in checked_packet:
        #print ("cw_data2;")
        if a < 9:
            slovo += znak
        if a > 8 and a < (delka - 8) :
            vec += znak
        a = a + 1
    #print(slovo)
    #print(vec)
    if slovo == "cw_data1;":
        final = vec
    elif slovo == "cw_data2;":
        final += vec
        print(final)
        with open("/home/pi/Desktop/Data.txt", "w") as file3:
            file3.write("%s" % final)
        final = ""
    checked_packet = ""
    
    

def doNothing():
    nothing = True

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
    packet = None                               #Variable for storing packets (set to NONE every loop)

    packet = rfm9x.receive(with_header=True)    #Listening for packets, Header - first 4 letters of message

    if packet != None:
        prev_packet = packet
        #print(prev_packet)
        with open("/home/pi/Desktop/received_data.txt", "a") as file1:
            file1.write("\n%s" % prev_packet)

        try:
            packet_text = str(prev_packet, "utf-8") #Decoding packet to an UTF-8 symbol package
        except:
            print("error during decoding")

            with open("/home/pi/Desktop/received_data.txt", "a") as file1:
                file1.write("\nerror during decoding")
                
        finally:
            vysledek = False
            ControlK()
            if vysledek == True:
                checked_packet = packet_text
                print(checked_packet)
            else:
                print("bad ck")
            

def DataWord():
    global packet_text

    dataword = ("")
    dataword += ("%s" % packet_text)

    #print (dataword)

    with open("/home/pi/Desktop/lora.txt", "w") as file:
        if file.writable():
            file.write(dataword)

def LoraSEND():
    counter = 0
    text = "cw_editRef;2000;"

    rfm9x.send(bytes("message number {}".format(text), "UTF-8"))
while True:
    commfromfile()
    LoraReceive()
    DataWord()
    uartcomm()
    


    #time.sleep(0.0001)
filecommfile.close()

