#!/usr/bin/perl  

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletIO16;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'gfR'; # Change to your UID

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $io = Tinkerforge::BrickletIO16->new(&UID, $ipcon); # Create device object

# Callback function for interrupts
sub cb_interrupt
{
    my ($port, $interrupt_mask, $value_mask) = @_;
    
    print "\nInterrupt on port: ".$port;
    print "\nInterrupt by: ".sprintf('%b', $interrupt_mask);
    print "\nValue: ".sprintf('%b', $value_mask)."\n";
}

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Register callback for interrupts
$io->register_callback($io->CALLBACK_INTERRUPT, 'cb_interrupt');

# Enable interrupt on pin 2 of port a
$io->set_port_interrupt('a', 1 << 2);

print "\nPress any key to exit...\n";
<STDIN>;
$ipcon->disconnect();

