#!/bin/sh
# connects to localhost:4223 by default, use --host and --port to change it

# change to your UID
uid=XYZ

# enable interrupt on pin 2 of port a: (1 << 2) = 4
tinkerforge call io16-bricklet $uid set-port-interrupt a 4

# handle incoming interrupt callbacks
tinkerforge dispatch io16-bricklet $uid interrupt
