import time
import board
import busio
import digitalio
import RPi.GPIO as io
import adafruit_rfm9x

sendpacket = "1"
checked_packet = ""

soucet = 0
vycet = 1
ck = ""
ckedit = 0


# setup interrupt callback function
def rfm9x_callback(rfm9x_irq):
    global packet_received
    global checked_packet
    # check to see if this was a rx interrupt - ignore tx
    if rfm9x.rx_done:
        packet = rfm9x.receive(with_header = True, timeout=None)
        if packet is not None:
            packet_received = True
            print("RSSI: {0}".format(rfm9x.last_rssi))
            #print(packet)
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
                if CK(packet_text):
                    #LoraSend()
                    checked_packet = packet_text
                    print(checked_packet)
                else:
                    print("bad ck")
    
RADIO_FREQ_MHZ = 433.0
CS = digitalio.DigitalInOut(board.CE1)
RESET = digitalio.DigitalInOut(board.D25)

spi = busio.SPI(board.SCK, MOSI=board.MOSI, MISO=board.MISO)

rfm9x = adafruit_rfm9x.RFM9x(spi, CS, RESET, RADIO_FREQ_MHZ)

rfm9x.tx_power = 23
rfm9x._write_u8(0xB9, 0xFF)


# configure the interrupt pin and event handling.
RFM9X_G0 = 5
io.setmode(io.BCM)
io.setup(RFM9X_G0, io.IN, pull_up_down=io.PUD_DOWN)  # activate input
io.add_event_detect(RFM9X_G0, io.RISING)
io.add_event_callback(RFM9X_G0, rfm9x_callback)

packet_received = False

rfm9x.send(bytes("GS Initializing", "utf-8"), keep_listening=True)

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

def CommandREAD():
    global sendpacket
            
    with open("/home/pi/Desktop/Command.txt", "r") as commandfile:
        if commandfile.readable():
            commandread = commandfile.read()
    if commandread == "":
        Command = "ACK;"
    elif commandread == "Restart;":
        restart()
        return
    else:
        Command = commandread
        
    soucet = 0
    for char in Command:
        soucet += int(ord(char))

    strsoucet = str(soucet)

    CK = ""
    for i in range(8 - (len(strsoucet))):
        CK += "0"
    CK += strsoucet


    sendpacket = Command
    sendpacket += CK
    
    #print(sendpacket)
def restart():
    Command = "Restart;"

    soucet = 0
    for char in Command:
        soucet += int(ord(char))

    strsoucet = str(soucet)

    CK = ""
    for i in range(8 - (len(strsoucet))):
        CK += "0"
    CK += strsoucet


    sendpacket = Command
    sendpacket += CK

    rfm9x.send(bytes(sendpacket, "UTF-8", keep_listening=True))
    print("sended")

    with open("/home/pi/Desktop/Command.txt", "w") as file1:
        file1.write("")
        
def LoraSend():
    global sendpacket
    print(sendpacket)
    rfm9x.send(bytes(sendpacket, "UTF-8"), keep_listening=True)
    print("sended")
    with open("/home/pi/Desktop/Command.txt", "w") as file1:
        file1.write("")
    
def Dataword():
    global checked_packet
    #print(checked_packet)
    if checked_packet != "":
        delka = len(checked_packet)
        final = checked_packet[:delka - 8]
        print(final)
        with open("/home/pi/Desktop/Lora.txt", "w") as lorafile:
            lorafile.write("%s" % final)
        final = ""
        checked_packet = ""
        
while True:
    time.sleep(0.1)
    CommandREAD()
    Dataword()
    if packet_received:
        packet_received = False
