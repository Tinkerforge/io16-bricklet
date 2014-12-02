using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "ABC"; // Change to your UID

	static void Main() 
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletIO16 io16 = new BrickletIO16(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Set pin 0 on port a to output low
		io16.SetPortConfiguration('a', 1 << 0, 'o', false);

		// Set pin 0 and 7 on port b to output high
		io16.SetPortConfiguration('b', (1 << 0) | (1 << 7), 'o', true);

		System.Console.WriteLine("Press enter to exit");
		System.Console.ReadLine();
		ipcon.Disconnect();
	}
}
