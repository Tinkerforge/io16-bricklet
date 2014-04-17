function octave_example_interrupt()
    more off;
    
    HOST = "localhost";
    PORT = 4223;
    UID = "goe"; % Change to your UID

    ipcon = java_new("com.tinkerforge.IPConnection"); % Create IP connection
    io = java_new("com.tinkerforge.BrickletIO16", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don"t use device before ipcon is connected

    % Register callback for interrupts
    io.addInterruptCallback(@cb_interrupt);

    % Enable interrupt on pin 2 of port a
    io.setPortInterrupt("a", bitshift(1, 2));

    input("Press any key to exit...\n", "s");
    ipcon.disconnect();
end

% Callback function for interrupts
function cb_interrupt(e)
    fprintf("Interrupt on port: %s\n", e.port);
    fprintf("Interrupt by: %s\n", dec2bin(str2num(e.interruptMask.toString())));
    fprintf("Value: %s\n", dec2bin(str2num(e.valueMask.toString())));
end
