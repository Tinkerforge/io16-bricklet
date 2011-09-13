import com.tinkerforge.BrickletIO16;
import com.tinkerforge.IPConnection;

public class ExampleOutput {
	private static final String host = new String("localhost");
	private static final int port = 4223;
	private static final String UID = new String("ABC"); // Change to your UID
	
	// Note: To make the example code cleaner we do not handle exceptions. Exceptions you
	//       might normally want to catch are described in the commnents below
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(host, port); // Create connection to brickd (Can throw IOException)

		BrickletIO16 io16 = new BrickletIO16(UID); // Create device object
		ipcon.addDevice(io16); // Add device to ip connection (Can throw IPConnection.TimeoutException)
		// Don't use device before it is added to a connection
		

		// Set pin 0 on port a to output low
		io16.setPortConfiguration('a', (short)(1 << 0), 'o', false);

		// Set pin 0 and 7 on port b to output high
		io16.setPortConfiguration('b', (short)((1 << 0) | (1 << 7)), 'o', true);
		
		System.out.println("Press ctrl+c to exit");
		ipcon.joinThread();
	}
}
