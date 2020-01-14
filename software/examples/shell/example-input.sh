#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your IO-16 Bricklet

# Get current value from port A as bitmask
tinkerforge call io16-bricklet $uid get-port "a"
