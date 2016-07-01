function octave_example_output()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your IO-16 Bricklet

    ipcon = java_new("com.tinkerforge.IPConnection"); % Create IP connection
    io = java_new("com.tinkerforge.BrickletIO16", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Set pin 0 on port A to output low
    io.setPortConfiguration("a", bitshift(1, 0), "o", false);

    % Set pin 0 and 7 on port B to output high
    io.setPortConfiguration("b", bitor(bitshift(1, 0), bitshift(1, 7)), "o", true);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end
