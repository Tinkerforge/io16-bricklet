function octave_example_input()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your IO-16 Bricklet

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    io = javaObject("com.tinkerforge.BrickletIO16", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current value from port A as bitmask
    valueMask = io.getPort("a");
    fprintf("Value Mask (Port A): %s\n", dec2bin(java2int(valueMask)));

    % Get current value from port B as bitmask
    valueMask = io.getPort("b");
    fprintf("Value Mask (Port B): %s\n", dec2bin(java2int(valueMask)));

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

function int = java2int(value)
    if compare_versions(version(), "3.8", "<=")
        int = value.intValue();
    else
        int = value;
    end
end
