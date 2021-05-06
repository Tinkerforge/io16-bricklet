using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your IO-16 Bricklet

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletIO16 io = new BrickletIO16(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get current value from port A as bitmask
		byte valueMask = io.GetPort('a');
		Console.WriteLine("Value Mask (Port A): " + Convert.ToString(valueMask, 2));

		// Get current value from port B as bitmask
		byte valueMask = io.GetPort('b');
		Console.WriteLine("Value Mask (Port B): " + Convert.ToString(valueMask, 2));

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
