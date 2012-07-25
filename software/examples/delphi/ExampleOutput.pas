program ExampleOutput;

{$ifdef MSWINDOWS}{$apptype CONSOLE}{$endif}
{$ifdef FPC}{$mode OBJFPC}{$H+}{$endif}

uses
  SysUtils, IPConnection, BrickletIO16;

type
  TExample = class
  private
    ipcon: TIPConnection;
    io: TBrickletIO16;
  public
    procedure Execute;
  end;

const
  HOST = 'localhost';
  PORT = 4223;
  UID = 'aDU';

var
  e: TExample;

procedure TExample.Execute;
begin
  { Create IP connection to brickd }
  ipcon := TIPConnection.Create(HOST, PORT);

  { Create device object }
  io := TBrickletIO16.Create(UID);

  { Add device to IP connection }
  ipcon.AddDevice(io);
  { Don't use device before it is added to a connection }

  { Set pin 0 on port a to output low }
  io.SetPortConfiguration('a', 1 shl 0, 'o', false);

  { Set pin 0 and 7 on port b to output high }
  io.SetPortConfiguration('b', (1 shl 0) or (1 shl 7), 'o', true);

  WriteLn('Press key to exit');
  ReadLn;
  ipcon.Destroy;
end;

begin
  e := TExample.Create;
  e.Execute;
  e.Destroy;
end.
