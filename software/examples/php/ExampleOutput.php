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

// Set pin 0 on port A to output low
$io->setPortConfiguration('a', 1 << 0, 'o', FALSE);

// Set pin 0 and 7 on port B to output high
$io->setPortConfiguration('b', (1 << 0) | (1 << 7), 'o', TRUE);

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));
$ipcon->disconnect();

?>
