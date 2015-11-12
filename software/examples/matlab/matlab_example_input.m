function matlab_example_input()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletIO16;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change to your UID

    ipcon = IPConnection(); % Create IP connection
    io = handle(BrickletIO16(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current value from port A as bitmask
    valueMask = io.getPort('a');
    fprintf('Value Mask (Port A): %s\n', dec2bin(valueMask));

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end
