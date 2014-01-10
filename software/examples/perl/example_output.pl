#!/usr/bin/perl  

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletIO16;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'gfR'; # Change to your UID

my $ipcon = IPConnection->new(); # Create IP connection
my $io = BrickletIO16->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Set pin 0 on port a to output low
$io->set_port_configuration('a', 1 << 0, 'o', 0);

# Set pin 0 and 7 on port b to output high
$io->set_port_configuration('b', (1 << 0) | (1 << 7), 'o', 1);

print "\nPress any key to exit...\n";
<STDIN>;
$ipcon->disconnect();

