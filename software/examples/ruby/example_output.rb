#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_io16'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change to your UID

ipcon = IPConnection.new # Create IP connection
io16 = BrickletIO16.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Set pin 0 on port a to output low
io16.set_port_configuration 'a', 1 << 0, 'o', false

# Set pin 0 and 7 on port b to output high
io16.set_port_configuration 'b', (1 << 0) | (1 << 7), 'o', true

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
