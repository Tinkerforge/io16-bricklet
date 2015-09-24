#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change to your UID

# Get current value from port A as bitmask
tinkerforge call io16-bricklet $uid get-port a
