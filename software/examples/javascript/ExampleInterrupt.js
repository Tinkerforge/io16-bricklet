var IPConnection = require('Tinkerforge/IPConnection');
var BrickletIO16 = require('Tinkerforge/BrickletIO16');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'gfp';// Change to your UID

var ipcon = new IPConnection();// Create IP connection
var io = new BrickletIO16(UID, ipcon);// Create device object

ipcon.connect(HOST, PORT,
    function(error) {
        console.log('Error: '+error);        
    }
);// Connect to brickd

// Don't use device before ipcon is connected
ipcon.on(IPConnection.CALLBACK_CONNECTED,
    function(connectReason) {
        //Enable interrupt on pin 2 of port a
        io.setPortInterrupt('a', 1 << 2);      
    }
);

// Register callback for interrupts
io.on(BrickletIO16.CALLBACK_INTERRUPT,
    // Callback function for interrupts
    function(port, interruptMask, valueMask) {
        console.log('Interrupt on port: '+port);
        console.log('Interrupt by: '+parseInt(interruptMask).toString(2));
        console.log('Value: '+parseInt(valueMask).toString(2));
        console.log();
    }
);

console.log("Press any key to exit ...");
process.stdin.on('data',
    function(data) {
        ipcon.disconnect();
        process.exit(0);
    }
);

