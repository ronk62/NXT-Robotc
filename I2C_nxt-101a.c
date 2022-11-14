//////////////////////////////////////////////////////////////////////////////////////////
//
//                           I2C Coding Source Code Examples
//                           ===============================
//
// Contains code snippets for working with the NXT I2C sensors.
//
////////////////////////////////////////////////////////////////////////////////////////

#pragma platform(NXT)

//////////////////////////////////////////////////////////////////////////////////////////
//
//             initializeI2CSensor(TSensors nPortIndex, const bool bFastI2C)
//
// Initializes an I2C sensor. 'bFastI2C' is a flag to indicate whether standard speed I2C
// (i.e. one byte transferred every 1 to 2 milliseconds using 12 KHz clock) or fast speed
// transmission should be used with the sensor.
//
// Fast speed is about five times faster than standard. It is suitable for sensors that
// have hardware I2C support rather than a bit-banged I2C implementation.
//
//////////////////////////////////////////////////////////////////////////////////////////

void initializeI2CSensor(const tSensors nPortIndex, const bool bFastI2C)
{
  SensorType[nPortIndex] = bFastI2C ? sensorI2CCustomFast : sensorI2CCustom;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
//                waitForI2CReply(TSensors nPortIndex, const int nReplyLength)
//
// Function will delay waiting for last I2C transaction to finish.
//
//////////////////////////////////////////////////////////////////////////////////////////

bool waitForI2CReply(tSensors nPortIndex, const int nReplyLength)
{
	while (nI2CStatus[nPortIndex] == STAT_COMM_PENDING)
		wait1Msec(1);

	if (nI2CBytesReady[nPortIndex] != nReplyLength)
		return false;

	return true;
}


//
// Alternate version showing more complicated error checking
//

bool checkErrorStatus(tSensors nPortIndex, const int nReplyLength)
{
	while (nI2CStatus[nPortIndex] == STAT_COMM_PENDING)
		wait1Msec(1); // Waiting for I2C transaction to complete

	//
	// Handle all possible return status. You may want to customize the
	// code for your particular situation.
	//
	switch (nI2CStatus[nPortIndex])
	{
	case NO_ERR:
		return true;

	case STAT_COMM_PENDING:
		// Shouldn't reach here because of preceding 'while' clause
		return false;

	case ERR_COMM_CHAN_NOT_READY:
		// Sensor is not configured for I2C messaging.
		return false;

	case ERR_COMM_BUS_ERR:
		// re-initialize sensor. An I2C messaging error occurred.
		return false;

	default:
		// "Illegal" value
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//
//                        Send a Message via I2C
//
// Sends an arbitrary 4-byte message over an I2C port. It would be easy to modify for
// messages of different length. Simply adjust the function parameters and the initialization
// of the 'nMsg' array.
//
// You'll have to poll the status of the I2C communications channel to tell when the
// I2C transaction is complete.
//
// Usually when writing to device the reply length will be zero.
//
//////////////////////////////////////////////////////////////////////////////////////////

bool sendI2CMessage(tSensors nPortIndex, int nReplyLength, byte nByte1, byte nByte2, byte nByte3, byte nByte4)
{
	const int kI2CAddress    = 0x02;    // You may want to make this a function parameter
	const int kRegisterIndex = 0x41;    // You may want to make this a function parameter

	const byte nMsg[] =
	{
		2 + 4,               // This is length field for transmitted message.
		kI2CAddress,         // The I2C address of the device. Almost all devices use value '0x02'
		kRegisterIndex,      // The internal register index within the sensor to start writing at.
		nByte1,
		nByte2,
		nByte3,
		nByte4
	};

	if (nI2CStatus[nPortIndex] == STAT_COMM_PENDING)
		return false;   // Can't send message to a busy device

	sendI2CMsg(nPortIndex, &nMsg[0], nReplyLength);
	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////
//
//                           Read from an I2C Device
//
// Sends a message to I2C sensor to read data from the device. You'll have to poll the
// status of the I2C communications channel to tell when the 'read' is complete.
//
//////////////////////////////////////////////////////////////////////////////////////////

bool sendSensorRead(tSensors nPortIndex, const ubyte &nSendMsg, const int nReplyLength)
{
	if (nI2CStatus[nPortIndex] == STAT_COMM_PENDING)
		return false;   // Can't send message to a busy device

	sendI2CMsg(nPortIndex, &nSendMsg, nReplyLength);
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
//                        readI2CReply(TSensors nPortIndex)
//
// Collects the reply bytes from an I2C message.
//
//////////////////////////////////////////////////////////////////////////////////////////

bool readSensorReply(tSensors nPortIndex, ubyte *pReplyMsg, const int nReplyLength)
{
	if (!waitForI2CReply(nPortIndex, nReplyLength))
	{
	  memset(pReplyMsg, 0, nReplyLength);
	  return false;
	}

	readI2CReply(nPortIndex, pReplyMsg, nReplyLength);
	return true;
}


////////////////////////////////////////////////////////////////////////////////////////////
//
//          Sample Task to Continuously read the sensor characteristics
//
////////////////////////////////////////////////////////////////////////////////////////////

task main()
{
	const int kI2CAddress    = 0x02;    // You may want to make this a function parameter
	const int kRegisterIndex = 0x00;    // You may want to make this a function parameter

	static const ubyte nReadDeviceID[] =
	{
		2,                   // This is length field for transmitted message.
		kI2CAddress,         // The I2C address of the device. Almost all devices use value '0x02'
		kRegisterIndex,      // The internal register index within the sensor to start writing at.
	};

	const int kReplyLength = 8;         // You might want to make this a function parameter.
	ubyte nReplyBytes[kReplyLength];

  bool bFastI2C;

  initializeI2CSensor(S1, false);
  while (true)
  {
    sendSensorRead(S1, nReadDeviceID[0], kReplyLength);
    while (!readSensorReply(S1, &nReplyBytes[0], kReplyLength))
    {}
    displayBigTextLine(0, "%s", nReplyBytes[0]);
  }
}
