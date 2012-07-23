import com.tinkerforge.BrickletIO16;
import com.tinkerforge.IPConnection;

public class ExampleInterrupt {
	private static final String host = "localhost";
	private static final int port = 4223;
	private static final String UID = "ABC"; // Change to your UID
	
	// Note: To make the example code cleaner we do not handle exceptions. Exceptions you
	//       might normally want to catch are described in the commnents below
	public static void main(String args[]) throws Exception {
		// Create connection to brickd
		IPConnection ipcon = new IPConnection(host, port); // Can throw IOException
		BrickletIO16 io16 = new BrickletIO16(UID); // Create device object

		// Add device to IP connection
		ipcon.addDevice(io16); // Can throw IPConnection.TimeoutException
		// Don't use device before it is added to a connection

		// Add and implement listener for interrupt (called if pin 2 of port a changes)
		io16.addListener(new BrickletIO16.InterruptListener() {
			public void interrupt(char port, short interruptMask, short valueMask) {
				System.out.println("Interrupt on port: " + port);
				System.out.println("Interrupt by: " + Integer.toBinaryString(interruptMask));
				System.out.println("Value: " + Integer.toBinaryString(valueMask));
			}
		});

		// Enable interrupt on pin 2 of port a
		io16.setPortInterrupt('a', (short)(1 << 2));

		System.console().readLine("Press key to exit\n");
		ipcon.destroy();
	}
}
