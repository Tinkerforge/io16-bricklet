use std::{error::Error, io, thread};
use tinkerforge::{io16_bricklet::*, ip_connection::IpConnection};

const HOST: &str = "localhost";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your IO-16 Bricklet.

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection.
    let io = Io16Bricklet::new(UID, &ipcon); // Create device object.

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd.
                                          // Don't use device before ipcon is connected.

    // Create receiver for interrupt events.
    let interrupt_receiver = io.get_interrupt_receiver();

    // Spawn thread to handle received events. This thread ends when the `io` object
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for interrupt in interrupt_receiver {
            println!("Port: {}", interrupt.port);
            println!("Interrupt Mask: {:b}", interrupt.interrupt_mask);
            println!("Value Mask: {:b}", interrupt.value_mask);
            println!();
        }
    });

    // Enable interrupt on pin 2 of port A
    io.set_port_interrupt('a', 1 << 2);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
