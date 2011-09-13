import com.tinkerforge.BrickletIO16;
import com.tinkerforge.IPConnection;

public class ExampleInterrupt {
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
		

		// Enable interrupt on pin 2 of port a
		io16.setPortInterrupt('a', (short)(1 << 2));

		// Add and implement listener for interrupt (called if pin 2 of port a changes)
		io16.addListener(new BrickletIO16.InterruptListener() {
			public void interrupt(char port, short interruptMask, short valueMask) {
				System.out.println("Interrupt on port: " + port);
				System.out.println("Interrupt by: " + Integer.toBinaryString(interruptMask));
				System.out.println("Value: " + Integer.toBinaryString(valueMask));
			}
		});
		
		System.out.println("Press ctrl+c to exit");
		ipcon.joinThread();
	}
}
