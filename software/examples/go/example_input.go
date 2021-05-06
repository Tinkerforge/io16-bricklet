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

	// Get current value from port A as bitmask.
	valueMaskA, _ := io.GetPort('a')
	fmt.Printf("Value Mask (Port A): %b\n", valueMaskA)

	// Get current value from port B as bitmask.
	valueMaskB, _ := io.GetPort('b')
	fmt.Printf("Value Mask (Port B): %b\n", valueMaskB)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()
}
