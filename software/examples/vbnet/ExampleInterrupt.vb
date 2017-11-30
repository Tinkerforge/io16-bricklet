Imports System
Imports Tinkerforge

Module ExampleInterrupt
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your IO-16 Bricklet

    ' Callback subroutine for interrupt callback
    Sub InterruptCB(ByVal sender As BrickletIO16, ByVal port As Char, _
                    ByVal interruptMask As Byte, ByVal valueMask As Byte)
        Console.WriteLine("Port: " + port)
        Console.WriteLine("Interrupt Mask: " + Convert.ToString(interruptMask, 2))
        Console.WriteLine("Value Mask: " + Convert.ToString(valueMask, 2))
        Console.WriteLine("")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim io As New BrickletIO16(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Register interrupt callback to subroutine InterruptCB
        AddHandler io.InterruptCallback, AddressOf InterruptCB

        ' Enable interrupt on pin 2 of port A
        io.SetPortInterrupt("a"C, 1 << 2)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
