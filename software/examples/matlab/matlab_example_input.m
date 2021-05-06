function matlab_example_input()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletIO16;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your IO-16 Bricklet

    ipcon = IPConnection(); % Create IP connection
    io = handle(BrickletIO16(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current value from port A as bitmask
    valueMaskA = io.getPort('a');
    fprintf('Value Mask (Port A): %s\n', dec2bin(valueMaskA));

    % Get current value from port B as bitmask
    valueMaskB = io.getPort('b');
    fprintf('Value Mask (Port B): %s\n', dec2bin(valueMaskB));

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end
