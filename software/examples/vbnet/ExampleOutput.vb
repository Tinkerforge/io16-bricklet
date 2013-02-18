Imports Tinkerforge

Module ExampleSimple
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "adL" ' Change to your UID

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim io16 As New BrickletIO16(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Set pin 0 on port a to output low
        io16.SetPortConfiguration("a"C, 1 << 0, "o"C, false)

        ' Set pin 0 and 7 on port b to output high
        io16.SetPortConfiguration("b"C, (1 << 0) or (1 << 7), "o"C, true)

        System.Console.WriteLine("Press key to exit")
        System.Console.ReadKey()
        ipcon.Disconnect()
    End Sub
End Module
