#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change to your UID

# Enable interrupt on pin 2 of port A: 1 << 2 = 4
tinkerforge call io16-bricklet $uid set-port-interrupt a 4

# Handle incoming interrupt callbacks
tinkerforge dispatch io16-bricklet $uid interrupt
