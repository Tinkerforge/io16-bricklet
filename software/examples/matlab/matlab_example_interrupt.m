function matlab_example_interrupt()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletIO16;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'goe'; % Change to your UID
    
    ipcon = IPConnection(); % Create IP connection
    io = BrickletIO16(UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register callback for interrupts
    set(io, 'InterruptCallback', @(h, e) cb_interrupt(e));

    % Enable interrupt on pin 2 of port a
    io.setPortInterrupt('a', bitshift(1, 2));

    input('Press any key to exit...\n', 's');
    ipcon.disconnect();
end

% Callback function for interrupts
function cb_interrupt(e)
    fprintf('Interrupt on port: %s\n', e.port);
    fprintf('Interrupt by: %s\n', dec2bin(e.interruptMask));
    fprintf('Value: %s\n', dec2bin(e.valueMask));
end
