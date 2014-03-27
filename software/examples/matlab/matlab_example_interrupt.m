function matlab_example_interrupt
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
    set(io, 'InterruptCallback', @(h, e)cb_interrupt(e.port, e.interruptMask, e.valueMask));

    % Enable interrupt on pin 2 of port a
    io.setPortInterrupt('a', bitshift(1, 2));

    input('\nPress any key to exit...\n', 's');
    ipcon.disconnect();
end

% Callback function for interrupts
function cb_interrupt(port, interrupt_mask, value_mask)
    fprintf('Interrupt on port: %s\n', port);
    fprintf('Interrupt by: %s\n', dec2bin(interrupt_mask));
    fprintf('Value: %s\n', dec2bin(value_mask));
end
