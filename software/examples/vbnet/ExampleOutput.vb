Imports System
Imports Tinkerforge

Module ExampleOutput
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your IO-16 Bricklet

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim io As New BrickletIO16(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Set pin 0 on port A to output low
        io.SetPortConfiguration("a"C, 1 << 0, "o"C, False)

        ' Set pin 0 and 7 on port B to output high
        io.SetPortConfiguration("b"C, (1 << 0) or (1 << 7), "o"C, True)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
