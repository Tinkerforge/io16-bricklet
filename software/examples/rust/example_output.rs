use std::{error::Error, io};

use tinkerforge::{io16_bricklet::*, ipconnection::IpConnection};

const HOST: &str = "127.0.0.1";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your IO-16 Bricklet

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection
    let io16_bricklet = IO16Bricklet::new(UID, &ipcon); // Create device object

    ipcon.connect(HOST, PORT).recv()??; // Connect to brickd
                                        // Don't use device before ipcon is connected

    // Set pin 0 on port A to output low
    io16_bricklet.set_port_configuration('a', 1 << 0, 'o', false);

    // Set pin 0 and 7 on port B to output high
    io16_bricklet.set_port_configuration('b', (1 << 0) | (1 << 7), 'o', true);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
