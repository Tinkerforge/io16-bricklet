#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_io16'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = '6VJ' # Change to your UID

ipcon = IPConnection.new # Create IP connection
io16 = BrickletIO16.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Register callback for interrupts
io16.register_callback(BrickletIO16::CALLBACK_INTERRUPT) do |port, interrupt_mask, value_mask|
  puts "Interrupt on port: #{port}"
  puts "Interrupt by: #{interrupt_mask.to_s 2}"
  puts "Value: #{value_mask.to_s 2}"
end

# Enable interrupt on pin 2 of port a
io16.set_port_interrupt 'a', 1 << 2

puts 'Press key to exit'
$stdin.gets
