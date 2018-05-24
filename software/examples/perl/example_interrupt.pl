#!/usr/bin/perl

use strict;
use Tinkerforge::IPConnection;
use Tinkerforge::BrickletIO16;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your IO-16 Bricklet

# Callback subroutine for interrupt callback
sub cb_interrupt
{
    my ($port, $interrupt_mask, $value_mask) = @_;

    print "Port: $port\n";
    print "Interrupt Mask: " . sprintf('%08b', $interrupt_mask) . "\n";
    print "Value Mask: " . sprintf('%08b', $value_mask) . "\n";
    print "\n";
}

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $io = Tinkerforge::BrickletIO16->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Register interrupt callback to subroutine cb_interrupt
$io->register_callback($io->CALLBACK_INTERRUPT, 'cb_interrupt');

# Enable interrupt on pin 2 of port A
$io->set_port_interrupt('a', 1 << 2);

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
