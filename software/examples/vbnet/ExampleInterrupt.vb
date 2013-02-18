Imports Tinkerforge

Module ExampleInterrupt
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "adL" ' Change to your UID

    ' Callback function for interrupts
    Sub InterruptCB(ByVal sender As BrickletIO16, _
                    ByVal port As Char, ByVal interruptMask As Byte, ByVal valueMask As Byte)
        Dim interruptBinary As String = System.Convert.ToString(interruptMask, 2)
        Dim valueBinary As String = System.Convert.ToString(valueMask, 2)

        System.Console.WriteLine("Interrupt on port: " + port)
        System.Console.WriteLine("Interrupt by: " + interruptBinary)
        System.Console.WriteLine("Value: " + valueBinary)
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim io16 As New BrickletIO16(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Register callback for interrupts
        AddHandler io16.Interrupt, AddressOf InterruptCB

        ' Enable interrupt on pin 2 of port a
        io16.SetPortInterrupt("a"C, 1 << 2)

        System.Console.WriteLine("Press key to exit")
        System.Console.ReadKey()
        ipcon.Disconnect()
    End Sub
End Module
