use std::{error::Error, io};

use tinkerforge::{io16_bricklet::*, ip_connection::IpConnection};

const HOST: &str = "localhost";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your IO-16 Bricklet.

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection.
    let io = Io16Bricklet::new(UID, &ipcon); // Create device object.

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd.
                                          // Don't use device before ipcon is connected.

    // Get current value from port A as bitmask.
    let value_mask_a = io.get_port('a').recv()?;
    println!("Value Mask (Port A): {:b}", value_mask_a);

    // Get current value from port B as bitmask.
    let value_mask_b = io.get_port('b').recv()?;
    println!("Value Mask (Port B): {:b}", value_mask_b);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
