var Tinkerforge = require('tinkerforge');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'gfp';// Change to your UID

var ipcon = new Tinkerforge.IPConnection();// Create IP connection
var io = new Tinkerforge.BrickletIO16(UID, ipcon);// Create device object

ipcon.connect(HOST, PORT,
    function(error) {
        console.log('Error: '+error);        
    }
);// Connect to brickd

// Don't use device before ipcon is connected
ipcon.on(Tinkerforge.IPConnection.CALLBACK_CONNECTED,
    function(connectReason) {
        // Set pin 0 on port a to output low
        io.setPortConfiguration('a', 1 << 0, 'o', false);
        // Set pin 0 and 7 on port b to output high
        io.setPortConfiguration('b', (1 << 0) | (1 << 7), 'o', true);      
    }
);

console.log("Press any key to exit ...");
process.stdin.on('data',
    function(data) {
        ipcon.disconnect();
        process.exit(0);
    }
);

