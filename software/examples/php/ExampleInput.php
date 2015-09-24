<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletIO16.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletIO16;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change to your UID

$ipcon = new IPConnection(); // Create IP connection
$io = new BrickletIO16(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Get current value from port A as bitmask
$value_mask = $io->getPort('a');
echo "Value Mask (Port A): " . sprintf("%08b", $value_mask) . "\n";

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));
$ipcon->disconnect();

?>
