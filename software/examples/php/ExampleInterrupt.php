<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletIO16.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletIO16;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your IO-16 Bricklet

// Callback function for interrupt callback
function cb_interrupt($port, $interrupt_mask, $value_mask)
{
    echo "Port: $port\n";
    echo "Interrupt Mask: " . sprintf("%08b", $interrupt_mask) . "\n";
    echo "Value Mask: " . sprintf("%08b", $value_mask) . "\n";
    echo "\n";
}

$ipcon = new IPConnection(); // Create IP connection
$io = new BrickletIO16(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Register interrupt callback to function cb_interrupt
$io->registerCallback(BrickletIO16::CALLBACK_INTERRUPT, 'cb_interrupt');

// Enable interrupt on pin 2 of port A
$io->setPortInterrupt('a', 1 << 2);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
