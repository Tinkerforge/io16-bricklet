#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_io16'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change to your UID

ipcon = IPConnection.new # Create IP connection
io = BrickletIO16.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Register interrupt callback
io.register_callback(BrickletIO16::CALLBACK_INTERRUPT) do |port, interrupt_mask, value_mask|
  puts "Port: #{port}"
  puts "Interrupt Mask: #{'%08b' % interrupt_mask}"
  puts "Value Mask: #{'%08b' % value_mask}"
  puts ''
end

# Enable interrupt on pin 2 of port A
io.set_port_interrupt 'a', 1 << 2

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
