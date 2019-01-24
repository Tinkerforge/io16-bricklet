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

	io.RegisterInterruptCallback(func(port rune, interruptMask uint8, valueMask uint8) {
		fmt.Printf("Port: %c\n", port)
		fmt.Printf("Interrupt Mask: %b\n", interruptMask)
		fmt.Printf("Value Mask: %b\n", valueMask)
		fmt.Println()
	})

	// Enable interrupt on pin 2 of port A
	io.SetPortInterrupt('a', 1<<2)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()
}
