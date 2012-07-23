<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletIO16.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletIO16;

$host = 'localhost';
$port = 4223;
$uid = '6VJ'; // Change to your UID

// Callback function for interrupts
function cb_interrupt($port, $interruptMask, $valueMask)
{
    echo "Interrupt on port: $port\n";
    echo "Interrupt by: " . decbin($interruptMask) . "\n";
    echo "Value: " . decbin($valueMask) . "\n";
}

$ipcon = new IPConnection($host, $port); // Create IP connection to brickd
$io16 = new BrickletIO16($uid); // Create device object

$ipcon->addDevice($io16); // Add device to IP connection
// Don't use device before it is added to a connection

// Register callback for interrupts
$io16->registerCallback(BrickletIO16::CALLBACK_INTERRUPT, 'cb_interrupt');

// Enable interrupt on pin 2 of port a
$io16->setPortInterrupt('a', 1 << 2);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
