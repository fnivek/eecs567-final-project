#!/usr/bin/env python

import serial
import os
import sys
import time

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
print 'Starting echo'
ser.write("start")
# Read until we see the finished text
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
finally:
    # Close the serial port
    ser.close()
