#!/usr/bin/env python

import serial
import os
import sys

# Make and flash the unit test
FILE_LOCATION =  os.path.dirname(os.path.abspath(__file__))
os.chdir(FILE_LOCATION + "/../")
print os.system("make flash_unit_test")

# Ask the user to reset the board
raw_input("\nPlease press the phsyical reset button on the STM32F4Discovery board and then press enter to continue...")

# Open a serial port
ser = serial.Serial("/dev/serial/by-id/usb-eecs567_final_project-if00", 115200)

# Send data to start USB OTG
ser.write("start")

# Read until we see the finished text
result = ''
try:
    while True:
        num_chars = ser.inWaiting()
        if num_chars:
            new = ser.read(num_chars)
            sys.stdout.write(new)
            result += new
            if result.find("Finished") != -1:
                break
finally:
    # Print the result so the user can see and close the serial port
    #print result
    ser.close()