#!/usr/bin/env python
# -*- coding: utf-8 -*-  

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change to your UID

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_io16 import IO16

# Callback function for interrupts
def cb_interrupt(port, interrupt_mask, value_mask):
    print('Interrupt on port: ' + port)
    print('Interrupt by: ' + str(bin(interrupt_mask)))
    print('Value: ' + str(bin(value_mask)))

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    io = IO16(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Register callback for interrupts
    io.register_callback(io.CALLBACK_INTERRUPT, cb_interrupt)

    # Enable interrupt on pin 2 of port a
    io.set_port_interrupt('a', 1 << 2)

    raw_input('Press key to exit\n') # Use input() in Python 3
    ipcon.disconnect()
