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
		wait1Msec(10);

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
	const int kI2CAddress    = 0x02;      // You may want to make this a function parameter
	const int kRegisterIndex41 = 0x41;    // You may want to make this a function parameter

	const byte nMsg[] =
	{
		2 + 4,               // This is length field for transmitted message.
		kI2CAddress,         // The I2C address of the device. Almost all devices use value '0x02'
		kRegisterIndex41,      // The internal register index within the sensor to start writing at.
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


//////////////////////////////////////////////////////////////////////////////////////////
//
//                        moveMotorA(int pwr, int target)
//
// Actuate motorA to initiate a right or left turn and return steering to center (approx)
//
//////////////////////////////////////////////////////////////////////////////////////////

void moveMotorA(int pwr, int target)
{
	nMotorEncoder[motorA]=0; //reset encoder
	nMotorEncoderTarget[motorA]=target;
	motor[motorA] = pwr;
	while(nMotorRunState[motorA]!=runStateIdle)// while Motor A is still running (hasn't reached target yet):

	{

		// do not continue

	}
	motor[motorA] = 0;

	wait1Msec(100); //wait before continuing

	nMotorEncoder[motorA]=0; //reset encoder
	nMotorEncoderTarget[motorA]= -1*target;
	motor[motorA] = -1*pwr;
	while(nMotorRunState[motorA]!=runStateIdle)// while Motor A is still running (hasn't reached target yet):

	{

		// do not continue

	}
	motor[motorA] = 0;

}



////////////////////////////////////////////////////////////////////////////////////////////
//
//          Sample Task to Continuously read the sensor characteristics
//
////////////////////////////////////////////////////////////////////////////////////////////

task main()
{
	const int kI2CAddress    = 0x02;     // You may want to make this a function parameter
	const int kRegisterIndex = 0x50;    // You may want to make this a function parameter

	const int kReplyLength = 6;          // You might want to make this a function parameter.
	ubyte nReplyBytes[kReplyLength];

	static const ubyte nReadDevice[] =
	{
		2,                   // This is length field for transmitted message.
		kI2CAddress,         // The I2C address of the device. Almost all devices use value '0x02'
		kRegisterIndex,     // The internal register index within the sensor to start writing at.
	};

	ubyte *pnReadDevice=&nReadDevice;

	bool bFastI2C;

	ubyte xPos;
	ubyte colorDetected;
	int noBlobCount=0;

	initializeI2CSensor(S1, false);

	bFloatDuringInactiveMotorPWM=false; //don't coast the motors

	while (true)
	{
		eraseDisplay();

		sendSensorRead(S1, nReadDevice[0], kReplyLength);
		while (!readSensorReply(S1, &nReplyBytes[0], kReplyLength))
		{
			//wait for read to return bool 'true'
			sleep(10);
		}

		for (int i=0; i<2; i++)
		{

			displayTextLine(i, "%x", nReplyBytes[i]);

			//sleep(20);
		}
		for (int i=2; i<6; i++)
		{

			displayTextLine(i, "%d", nReplyBytes[i]);

			//sleep(20);
		}

		//set vars to nReplyBytes values
		colorDetected=nReplyBytes[0];
		xPos=nReplyBytes[2];

		//evaluate detected blob color and location; actuate motor as needed
		if (colorDetected==0x01 && xPos>140) moveMotorA(45,90); //turn left
		if (colorDetected>0x01 && xPos<190) moveMotorA(45,90); //turn left
		if (colorDetected==0x01 && xPos<90) moveMotorA(-45,-90); //turn right
		if (colorDetected==0x00) noBlobCount++; //no blob detected - increment noBlobCount
		if (noBlobCount>14)
    {
      moveMotorA(45,90); //turn left
      noBlobCount=0; //reset count to zero
    }

		sleep(200);
	}
}
