using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your IO-16 Bricklet

	// Callback function for interrupt callback
	static void InterruptCB(BrickletIO16 sender, char port, byte interruptMask,
	                        byte valueMask)
	{
		Console.WriteLine("Port: " + port);
		Console.WriteLine("Interrupt Mask: " + Convert.ToString(interruptMask, 2));
		Console.WriteLine("Value Mask: " + Convert.ToString(valueMask, 2));
		Console.WriteLine("");
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletIO16 io = new BrickletIO16(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Register interrupt callback to function InterruptCB
		io.InterruptCallback += InterruptCB;

		// Enable interrupt on pin 2 of port A
		io.SetPortInterrupt('a', 1 << 2);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
