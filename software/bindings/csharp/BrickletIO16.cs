/*************************************************************
 * This file was automatically generated on 2011-09-20.      *
 *                                                           *
 * If you have a bugfix for this file and want to commit it, *
 * please fix the bug in the generator. You can find a link  *
 * to the generator git on tinkerforge.com                   *
 *************************************************************/

namespace Tinkerforge
{
	public class BrickletIO16 : Device 
	{
		private static byte TYPE_SET_PORT = 1;
		private static byte TYPE_GET_PORT = 2;
		private static byte TYPE_SET_PORT_CONFIGURATION = 3;
		private static byte TYPE_GET_PORT_CONFIGURATION = 4;
		private static byte TYPE_SET_DEBOUNCE_PERIOD = 5;
		private static byte TYPE_GET_DEBOUNCE_PERIOD = 6;
		private static byte TYPE_SET_PORT_INTERRUPT = 7;
		private static byte TYPE_GET_PORT_INTERRUPT = 8;
		private static byte TYPE_INTERRUPT = 9;

		public delegate void Interrupt(char port, byte interruptMask, byte valueMask);

		public BrickletIO16(string uid) : base(uid) 
		{
			messageCallbacks[TYPE_INTERRUPT] = new MessageCallback(CallbackInterrupt);
		}

		public void SetPort(char port, byte valueMask)
		{
			byte[] data = new byte[6];
			LEConverter.To(stackID, 0, data);
			LEConverter.To(TYPE_SET_PORT, 1, data);
			LEConverter.To((ushort)6, 2, data);
			LEConverter.To(port, 4, data);
			LEConverter.To(valueMask, 5, data);

			ipcon.Write(this, data, TYPE_SET_PORT, false);
		}

		public void GetPort(char port, out byte valueMask)
		{
			byte[] data = new byte[5];
			LEConverter.To(stackID, 0, data);
			LEConverter.To(TYPE_GET_PORT, 1, data);
			LEConverter.To((ushort)5, 2, data);
			LEConverter.To(port, 4, data);

			ipcon.Write(this, data, TYPE_GET_PORT, true);

			byte[] answer;
			if(!answerQueue.TryDequeue(out answer, IPConnection.TIMEOUT_ANSWER))
			{
				throw new TimeoutException("Did not receive answer for GetPort in time");
			}

			valueMask = LEConverter.ByteFrom(4, answer);

			writeEvent.Set();
		}

		public void SetPortConfiguration(char port, byte portMask, char direction, bool value)
		{
			byte[] data = new byte[8];
			LEConverter.To(stackID, 0, data);
			LEConverter.To(TYPE_SET_PORT_CONFIGURATION, 1, data);
			LEConverter.To((ushort)8, 2, data);
			LEConverter.To(port, 4, data);
			LEConverter.To(portMask, 5, data);
			LEConverter.To(direction, 6, data);
			LEConverter.To(value, 7, data);

			ipcon.Write(this, data, TYPE_SET_PORT_CONFIGURATION, false);
		}

		public void GetPortConfiguration(char port, out byte directionMask, out byte valueMask)
		{
			byte[] data = new byte[5];
			LEConverter.To(stackID, 0, data);
			LEConverter.To(TYPE_GET_PORT_CONFIGURATION, 1, data);
			LEConverter.To((ushort)5, 2, data);
			LEConverter.To(port, 4, data);

			ipcon.Write(this, data, TYPE_GET_PORT_CONFIGURATION, true);

			byte[] answer;
			if(!answerQueue.TryDequeue(out answer, IPConnection.TIMEOUT_ANSWER))
			{
				throw new TimeoutException("Did not receive answer for GetPortConfiguration in time");
			}

			directionMask = LEConverter.ByteFrom(4, answer);
			valueMask = LEConverter.ByteFrom(5, answer);

			writeEvent.Set();
		}

		public void SetDebouncePeriod(uint debounce)
		{
			byte[] data = new byte[8];
			LEConverter.To(stackID, 0, data);
			LEConverter.To(TYPE_SET_DEBOUNCE_PERIOD, 1, data);
			LEConverter.To((ushort)8, 2, data);
			LEConverter.To(debounce, 4, data);

			ipcon.Write(this, data, TYPE_SET_DEBOUNCE_PERIOD, false);
		}

		public void GetDebouncePeriod(out uint debounce)
		{
			byte[] data = new byte[4];
			LEConverter.To(stackID, 0, data);
			LEConverter.To(TYPE_GET_DEBOUNCE_PERIOD, 1, data);
			LEConverter.To((ushort)4, 2, data);

			ipcon.Write(this, data, TYPE_GET_DEBOUNCE_PERIOD, true);

			byte[] answer;
			if(!answerQueue.TryDequeue(out answer, IPConnection.TIMEOUT_ANSWER))
			{
				throw new TimeoutException("Did not receive answer for GetDebouncePeriod in time");
			}

			debounce = LEConverter.UIntFrom(4, answer);

			writeEvent.Set();
		}

		public void SetPortInterrupt(char port, byte interruptMask)
		{
			byte[] data = new byte[6];
			LEConverter.To(stackID, 0, data);
			LEConverter.To(TYPE_SET_PORT_INTERRUPT, 1, data);
			LEConverter.To((ushort)6, 2, data);
			LEConverter.To(port, 4, data);
			LEConverter.To(interruptMask, 5, data);

			ipcon.Write(this, data, TYPE_SET_PORT_INTERRUPT, false);
		}

		public void GetPortInterrupt(char port, out byte interruptMask)
		{
			byte[] data = new byte[5];
			LEConverter.To(stackID, 0, data);
			LEConverter.To(TYPE_GET_PORT_INTERRUPT, 1, data);
			LEConverter.To((ushort)5, 2, data);
			LEConverter.To(port, 4, data);

			ipcon.Write(this, data, TYPE_GET_PORT_INTERRUPT, true);

			byte[] answer;
			if(!answerQueue.TryDequeue(out answer, IPConnection.TIMEOUT_ANSWER))
			{
				throw new TimeoutException("Did not receive answer for GetPortInterrupt in time");
			}

			interruptMask = LEConverter.ByteFrom(4, answer);

			writeEvent.Set();
		}

		public int CallbackInterrupt(byte[] data)
		{
			char port = LEConverter.CharFrom(4, data);
			byte interruptMask = LEConverter.ByteFrom(5, data);
			byte valueMask = LEConverter.ByteFrom(6, data);

			((Interrupt)callbacks[TYPE_INTERRUPT])(port, interruptMask, valueMask);
			return 7;
		}

		public void RegisterCallback(System.Delegate d)
		{
			if(d.GetType() == typeof(Interrupt))
			{
				callbacks[TYPE_INTERRUPT] = d;
			}
		}
	}
}
