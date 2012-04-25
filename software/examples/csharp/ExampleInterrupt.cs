using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "ABC"; // Change to your UID

	// Callback function for interrupts
	static void InterruptCB(char port, byte interruptMask, byte valueMask)
	{
		string interruptBinary = System.Convert.ToString(interruptMask, 2);
		string valueBinary = System.Convert.ToString(valueMask, 2);

		System.Console.WriteLine("Interrupt on port: " + port);
		System.Console.WriteLine("Interrupt by: " + interruptBinary);
		System.Console.WriteLine("Value: " + valueBinary);
	}

	static void Main() 
	{
		IPConnection ipcon = new IPConnection(HOST, PORT); // Create connection to brickd
		BrickletIO16 io16 = new BrickletIO16(UID); // Create device object
		ipcon.AddDevice(io16); // Add device to IP connection
		// Don't use device before it is added to a connection

		// Enable interrupt on pin 2 of port a
		io16.SetPortInterrupt('a', 1 << 2);

		// Register callback for interrupts
		io16.RegisterCallback(new BrickletIO16.Interrupt(InterruptCB));

		System.Console.WriteLine("Press ctrl+c to exit");
		ipcon.JoinThread();
	}
}
