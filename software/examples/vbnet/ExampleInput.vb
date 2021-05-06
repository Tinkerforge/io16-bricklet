Imports System
Imports Tinkerforge

Module ExampleInput
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your IO-16 Bricklet

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim io As New BrickletIO16(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Get current value from port A as bitmask
        Dim valueMaskA As Byte = io.GetPort("a"C)
        Console.WriteLine("Value Mask (Port A): " + Convert.ToString(valueMaskA, 2))

        ' Get current value from port B as bitmask
        Dim valueMaskB As Byte = io.GetPort("b"C)
        Console.WriteLine("Value Mask (Port B): " + Convert.ToString(valueMaskB, 2))

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
