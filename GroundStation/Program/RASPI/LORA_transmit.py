# SPDX-FileCopyrightText: 2018 Brent Rubell for Adafruit Industries
#
# SPDX-License-Identifier: MIT

"""
Example for using the RFM9x Radio with Raspberry Pi.

Learn Guide: https://learn.adafruit.com/lora-and-lorawan-for-raspberry-pi
Author: Brent Rubell for Adafruit Industries
"""
# Import Python System Libraries
import time
# Import Blinka Libraries
import busio
from digitalio import DigitalInOut
import board
# Import RFM9x
import adafruit_rfm9x


# Configure LoRa Radio
CS = DigitalInOut(board.CE1)
RESET = DigitalInOut(board.D25)
spi = busio.SPI(board.SCK, MOSI=board.MOSI, MISO=board.MISO)
rfm9x = adafruit_rfm9x.RFM9x(spi, CS, RESET, 433.0)
rfm9x.tx_power = 23
prev_packet = None

while True:
    packet = None

    # check for packet rx
    packet = rfm9x.receive(with_header=True)
    if packet is None:
        print('- Waiting for PKT -')
    else:
        # Display the packet text and rssi
        prev_packet = packet
        #print(prev_packet)
        packet_text = str(prev_packet, "utf-8")
        print('RX: ')
        print(packet_text)
        #-------------------
        #print("Received (raw header):", [hex(x) for x in packet[0:4]])
        #print("Received (raw payload): {0}".format(packet[4:]))
        #print("RSSI: {0}".format(rfm9x.last_rssi))
        time.sleep(0.1)
    time.sleep(0.1)
