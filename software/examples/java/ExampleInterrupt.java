import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletIO16;

public class ExampleInterrupt {
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

		// Add interrupt listener
		io.addInterruptListener(new BrickletIO16.InterruptListener() {
			public void interrupt(char port, short interruptMask, short valueMask) {
				System.out.println("Port: " + port);
				System.out.println("Interrupt Mask: " + Integer.toBinaryString(interruptMask));
				System.out.println("Value Mask: " + Integer.toBinaryString(valueMask));
				System.out.println("");
			}
		});

		// Enable interrupt on pin 2 of port A
		io.setPortInterrupt('a', (short)(1 << 2));

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
