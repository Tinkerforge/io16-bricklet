#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_io16'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = '6VJ' # Change to your UID

ipcon = IPConnection.new HOST, PORT # Create IP connection to brickd
io16 = BrickletIO16.new UID # Create device object
ipcon.add_device io16 # Add device to IP connection
# Don't use device before it is added to a connection

# Set pin 0 on port a to output low
io16.set_port_configuration 'a', 1 << 0, 'o', false

# Set pin 0 and 7 on port b to output high
io16.set_port_configuration 'b', (1 << 0) | (1 << 7), 'o', false

puts 'Press key to exit'
$stdin.gets
ipcon.destroy