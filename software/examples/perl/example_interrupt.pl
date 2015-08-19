#!/usr/bin/perl

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletIO16;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change to your UID

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $io16 = Tinkerforge::BrickletIO16->new(&UID, $ipcon); # Create device object

# Callback function for interrupts
sub cb_interrupt
{
    my ($port, $interrupt_mask, $value_mask) = @_;
    
    print "Interrupt on port: $port\n";
    print "Interrupt by: ".sprintf('%b', $interrupt_mask)."\n";
    print "Value: ".sprintf('%b', $value_mask)."\n";
}

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Register callback for interrupts
$io16->register_callback($io16->CALLBACK_INTERRUPT, 'cb_interrupt');

# Enable interrupt on pin 2 of port a
$io16->set_port_interrupt('a', 1 << 2);

print "Press any key to exit...\n";
<STDIN>;
$ipcon->disconnect();
