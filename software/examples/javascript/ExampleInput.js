var Tinkerforge = require('tinkerforge');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'XYZ'; // Change XYZ to the UID of your IO-16 Bricklet

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
        // Get current value from port A as bitmask
        io.getPort('a',
            function (valueMask) {
                console.log('Value Mask (Port A): ' + valueMask.toString(2));
            },
            function (error) {
                console.log('Error: ' + error);
            }
        );

        // Get current value from port B as bitmask
        io.getPort('b',
            function (valueMask) {
                console.log('Value Mask (Port B): ' + valueMask.toString(2));
            },
            function (error) {
                console.log('Error: ' + error);
            }
        );
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
