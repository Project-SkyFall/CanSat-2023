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
rfm9x.tx_power = 23
prev_packet = None                              #Reset main storing variable
packet_text = None

def doNothing():
    nothing = True

def LoraReceive():                              #Basic lora function
    global packet_text
    global prev_packet                          #Definition of variables
    packet = None                               #Variable for storing packets (set to NONE every loop)

    packet = rfm9x.receive(with_header=True)    #Listening for packets, Header - first 4 letters of message
    if packet is None:
        #print('- Waiting for PKT -')
        doNothing()
    else:
        prev_packet = packet                    #Storing the received packet for further using
        #print(prev_packet)
        packet_text = str(prev_packet, "utf-8") #Decoding packet to an UTF-8 symbol package
        #print('RX: ')
        print(packet_text)                      #Printing decoded text
        #-------------------
        #print("Received (raw header):", [hex(x) for x in packet[0:4]])
        #print("Received (raw payload): {0}".format(packet[4:]))
        #print("RSSI: {0}".format(rfm9x.last_rssi))
        time.sleep(0.1)
    time.sleep(0.1)

def DataWord():
    global packet_text

    dataword = ("")
    dataword += ("%s" % packet_text)

    print (dataword)

    with open("/home/pi/Desktop/lora.txt", "w") as file:
        if file.writable():
            file.write(dataword)
    


while True:
    LoraReceive()
    DataWord()

    time.sleep(0.1)
