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
print 'Connecting to /dev/serial/by-id/usb-eecs567_final_project-if00'
tries = 0
while True:
    try:
        ser = serial.Serial("/dev/serial/by-id/usb-eecs567_final_project-if00", 115200)
        break
    except Exception as e:
        print 'Failed to connect to device waiting 3 seconds then trying again'
        print e
        tries += 1
        if tries >= 10:
            print 'Failed to connect to device 10 times exiting now'
            sys.exit()

        time.sleep(3)

# Send data to start USB OTG
print 'Write s'
ser.write("s")

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
            if result.find("(y/n)") != -1:
                user_in = ''
                while user_in != 'y' and user_in != 'n':
                    user_in = raw_input()
                ser.write(user_in)
                result = ''
finally:
    # Close the serial port
    ser.close()
