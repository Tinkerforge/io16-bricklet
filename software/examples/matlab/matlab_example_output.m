function matlab_example_output()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletIO16;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change to your UID

    ipcon = IPConnection(); % Create IP connection
    io = BrickletIO16(UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Set pin 0 on port A to output low
    io.setPortConfiguration('a', bitshift(1, 0), 'o', false);

    % Set pin 0 and 7 on port B to output high
    io.setPortConfiguration('b', bitor(bitshift(1, 0), bitshift(1, 7)), 'o', true);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end
