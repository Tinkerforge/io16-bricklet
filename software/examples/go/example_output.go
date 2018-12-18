package main

import (
	"fmt"
	"github.com/Tinkerforge/go-api-bindings/io16_bricklet"
	"github.com/Tinkerforge/go-api-bindings/ipconnection"
)

const ADDR string = "localhost:4223"
const UID string = "XYZ" // Change XYZ to the UID of your IO-16 Bricklet.

func main() {
	ipcon := ipconnection.New()
	defer ipcon.Close()
	io, _ := io16_bricklet.New(UID, &ipcon) // Create device object.

	ipcon.Connect(ADDR) // Connect to brickd.
	defer ipcon.Disconnect()
	// Don't use device before ipcon is connected.

	// Set pin 0 on port A to output low
	io.SetPortConfiguration('a', 1<<0, 'o', false)

	// Set pin 0 and 7 on port B to output high
	io.SetPortConfiguration('b', (1<<0)|(1<<7), 'o', true)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()

}
