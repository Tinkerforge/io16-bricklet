#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_io16'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change XYZ to the UID of your IO-16 Bricklet

ipcon = IPConnection.new # Create IP connection
io = BrickletIO16.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Get current value from port A as bitmask
value_mask = io.get_port 'a'
puts "Value Mask (Port A): #{'%08b' % value_mask}"

# Get current value from port B as bitmask
value_mask = io.get_port 'b'
puts "Value Mask (Port B): #{'%08b' % value_mask}"

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
