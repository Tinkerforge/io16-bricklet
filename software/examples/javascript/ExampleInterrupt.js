var Tinkerforge = require('tinkerforge');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'XYZ'; // Change to your UID

var ipcon = new Tinkerforge.IPConnection(); // Create IP connection
var io = new Tinkerforge.BrickletIO16(UID, ipcon); // Create device object

ipcon.connect(HOST, PORT,
    function (error) {
        console.log('Error: ' + error);
    }
); // Connect to brickd
// Don't use device before ipcon is connected

ipcon.on(Tinkerforge.IPConnection.CALLBACK_CONNECTED,
    function (connectReason) {
        // Enable interrupt on pin 2 of port A
        io.setPortInterrupt('a', 1 << 2);
    }
);

// Register interrupt callback
io.on(Tinkerforge.BrickletIO16.CALLBACK_INTERRUPT,
    // Callback function for interrupt callback
    function (port, interruptMask, valueMask) {
        console.log('Port: ' + port);
        console.log('Interrupt Mask: ' + interruptMask.toString(2));
        console.log('Value Mask: ' + valueMask.toString(2));
        console.log();
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
