<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletIO16.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletIO16;

const HOST = 'localhost';
const PORT = 4223;
const UID = '6VJ'; // Change to your UID

// Callback function for interrupts
function cb_interrupt($port, $interruptMask, $valueMask)
{
    echo "Interrupt on port: $port\n";
    echo "Interrupt by: " . decbin($interruptMask) . "\n";
    echo "Value: " . decbin($valueMask) . "\n";
}

$ipcon = new IPConnection(); // Create IP connection
$io16 = new BrickletIO16(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Register callback for interrupts
$io16->registerCallback(BrickletIO16::CALLBACK_INTERRUPT, 'cb_interrupt');

// Enable interrupt on pin 2 of port a
$io16->setPortInterrupt('a', 1 << 2);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
