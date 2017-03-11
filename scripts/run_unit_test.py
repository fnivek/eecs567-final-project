#!/usr/bin/env python

import serial
import os
import sys
import time

# Make and flash the unit test
FILE_LOCATION = os.path.dirname(os.path.abspath(__file__))
os.chdir(FILE_LOCATION + "/../")
print os.system("make flash_unit_test")

# Ask the user to reset the board
raw_input("""\nPlease press the phsyical reset button on
           the STM32F4Discovery board and then press enter to continue...""")

# Open a serial port
time.sleep(1)
print 'Connecting to /dev/serial/by-id/usb-eecs567_final_project-if00'
ser = serial.Serial("/dev/serial/by-id/usb-eecs567_final_project-if00", 115200)
# time.sleep(1)

# Send data to start USB OTG
print 'Write start'
ser.write("start")

print 'Run test'
# Read until we see the finished text
result = ''
try:
    while True:
        num_chars = ser.inWaiting()
        if num_chars:
            new = ''
            try:
                new = ser.read(num_chars)
            except:
                print '\nFailed to read'
            sys.stdout.write(new)
            result += new
            if result.find("Finished") != -1:
                break
finally:
    # Close the serial port
    ser.close()
