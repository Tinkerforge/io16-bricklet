#!/bin/sh
# connects to localhost:4223 by default, use --host and --port to change it

# change to your UID
uid=XYZ

# set pin 0 on port a to output low: (1 << 0) = 1
tinkerforge call io16-bricklet $uid set-port-configuration a 1 out false

# set pin 0 and 7 on port b to output high: (1 << 0) | (1 << 7) = 129
tinkerforge call io16-bricklet $uid set-port-configuration b 129 out true
