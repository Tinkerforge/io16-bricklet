<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletIO16.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletIO16;

$host = 'localhost';
$port = 4223;
$uid = '6VJ'; // Change to your UID

$ipcon = new IPConnection(); // Create IP connection
$io16 = new BrickletIO16($uid, $ipcon); // Create device object

$ipcon->connect($host, $port); // Connect to brickd
// Don't use device before ipcon is connected

// Set pin 0 on port a to output low
$io16->setPortConfiguration('a', 1 << 0, 'o', FALSE);

// Set pin 0 and 7 on port b to output high
$io16->setPortConfiguration('b', (1 << 0) | (1 << 7), 'o', TRUE);

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));

?>
