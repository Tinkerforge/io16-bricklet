import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletIO16;

public class ExampleOutput {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;
	private static final String UID = "XYZ"; // Change to your UID

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions
	//       you might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletIO16 io = new BrickletIO16(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Set pin 0 on port A to output low
		io.setPortConfiguration('a', (short)(1 << 0), 'o', false);

		// Set pin 0 and 7 on port B to output high
		io.setPortConfiguration('b', (short)((1 << 0) | (1 << 7)), 'o', true);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
