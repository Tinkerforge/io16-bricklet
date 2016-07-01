#!/usr/bin/perl

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletIO16;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your IO-16 Bricklet

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $io = Tinkerforge::BrickletIO16->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Get current value from port A as bitmask
my $value_mask = $io->get_port('a');
print "Value Mask (Port A): " . sprintf('%08b', $value_mask) . "\n";

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
