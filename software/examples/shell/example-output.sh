#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your IO-16 Bricklet

# Set pin 0 on port A to output low: 1 << 0 = 1
tinkerforge call io16-bricklet $uid set-port-configuration "a" 1 direction-out false

# Set pin 0 and 7 on port B to output high: (1 << 0) | (1 << 7) = 129
tinkerforge call io16-bricklet $uid set-port-configuration "b" 129 direction-out true
