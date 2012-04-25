#!/usr/bin/env python
# -*- coding: utf-8 -*-  

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change to your UID

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_io16 import IO16

if __name__ == "__main__":
    ipcon = IPConnection(HOST, PORT) # Create IP connection to brickd

    io = IO16(UID) # Create device object
    ipcon.add_device(io) # Add device to IP connection
    # Don't use device before it is added to a connection

    # Set pin 0 on port a to output low
    io.set_port_configuration('a', 1 << 0, 'o', False)

    # Set pin 0 and 7 on port b to output high
    io.set_port_configuration('b', (1 << 0) | (1 << 7), 'o', True)

    raw_input('Press key to exit\n') # Use input() in Python 3
    ipcon.destroy()
