<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletIO16.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletIO16;

$host = 'localhost';
$port = 4223;
$uid = '6VJ'; // Change to your UID

$ipcon = new IPConnection($host, $port); // Create IP connection to brickd
$io16 = new BrickletIO16($uid); // Create device object

$ipcon->addDevice($io16); // Add device to IP connection
// Don't use device before it is added to a connection

// Set pin 0 on port a to output low
$io16->setPortConfiguration('a', 1 << 0, 'o', FALSE);

// Set pin 0 and 7 on port b to output high
$io16->setPortConfiguration('b', (1 << 0) | (1 << 7), 'o', TRUE);

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));
$ipcon->destroy();

?>
