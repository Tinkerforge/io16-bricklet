function matlab_example_interrupt()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletIO16;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your IO-16 Bricklet

    ipcon = IPConnection(); % Create IP connection
    io = handle(BrickletIO16(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register interrupt callback to function cb_interrupt
    set(io, 'InterruptCallback', @(h, e) cb_interrupt(e));

    % Enable interrupt on pin 2 of port A
    io.setPortInterrupt('a', bitshift(1, 2));

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end

% Callback function for interrupt callback
function cb_interrupt(e)
    fprintf('Port: %s\n', e.port);
    fprintf('Interrupt Mask: %s\n', dec2bin(e.interruptMask));
    fprintf('Value Mask: %s\n', dec2bin(e.valueMask));
    fprintf('\n');
end
