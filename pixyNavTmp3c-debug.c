#pragma config(Sensor, S1,     PIXYC,          sensorI2CCustom)
#pragma config(Motor,  motorB,          LeftMotor,     tmotorNXT, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorC,          RightMotor,    tmotorNXT, PIDControl, driveRight, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/**
* This program uses the pixycam to seek, then close in on a pair of Signature coded
* objects.
*
* The yPos of the object indicates near/far proximity. The pixycam is mounted to
* a differential drive platform, which will move according to the pixycam values
*
* The current version will detect and close-in on one or both objects (sig1 abd sig2)
* including a mode where if both objects are detected, the bot will close-in on the
* virtual center (xPos and yPos) of the pair.
*
* The next iteration will add a set of routines to orient the bot in such a way that
* a third object/sig3 can be placed such that it apears in an x/y position in the frame
* that indicates a location on a grid.
*
* Revisions:
* Tmp2  - initial development version
* Tmp3  - added corrections for a bug when transitioning from no objects detected
*				- added for/next loop to take 5 readins per cycle to help with avg'ing/smoothing
*				- additional corrections and cleanup
*/

#include "pixyCamV1-lib-RK1b.h"


int rangeLim(int val, int lim) //(abs(val+lim)-abs(val-lim))/2 ...returns vals in range of +/- lim
{
	val = (abs(val+lim)-abs(val-lim))/2;
	return val;
}

int scaleInput(float val, float maxIn, float maxOut) // scale input values based on ratio
{
	val = val/(maxIn/maxOut);
	return val;
}


task main()
{
	// initialize pixycam and motor pwr values
	ubyte _qryAddress = 0x51;   // The address of the Signature (51-57)
	ubyte _numberOfBlocks=0;
	ubyte numberOfBlocksS1=0;
	ubyte numberOfBlocksS2=0;
	ubyte numberOfBlocksFlag=1;
	ubyte _xPos=0;
	int offset_xPos=0;
	int scaled_xPos=0;
	ubyte _yPos=0;
	ubyte _width=0;
	ubyte _height=0;
	// int area=0; // had to use 'int', since ubyte tops out at 255

	float xAvgS1=0.0;
	float yAvgS1=0.0;
	float xAvgS2=0.0;
	float yAvgS2=0.0;
	float xAvgSn=0.0;
	float yAvgSn=0.0;
	float xDelta=0.0;
	float yDelta=0.0;

	int motorBpwr=0;
	int motorCpwr=0;
	int topSpeedLim=22;  // set this to a very low value for initial testing
	int outputMax=22;    // max motor power
	int inputMax=126;    // val set to 255/2 since we are using centered '0' and '-125' offset
	int fwdRevPwr=0;     // controls fwd/rev movement (direction and speed)
	int K=1;             // coefficient for rotation speed

	// initialize pixycam
	initializeI2CSensor(PIXYC, false);
	sleep(150);

	clearTimer(T1); // clear (reset) timer 1, which is used to detect how long since last object detected


	/**
	* Main while loop
	* DiffDrv Platform follows _xPos from pixycam
	*
	*/

	while (true) {

		for (_qryAddress = 0x51; _qryAddress < 0x53; _qryAddress ++)
		{
			// get 5 readings each cycle and avg results before using to control motors
		  for (int i = 1; i < 6; i ++)
			{
				while (!PIXYCreadSignatureQry(PIXYC, _qryAddress, _numberOfBlocks, _xPos, _yPos, _width, _height))
				{
					//wait for read to return bool 'true'
					sleep(50);
				}

				if (_qryAddress == 0x51)
				{
					numberOfBlocksS1 = _numberOfBlocks;
					xAvgS1 = 0.75 * xAvgS1 + 0.25 * _xPos;
					yAvgS1 = 0.75 * yAvgS1 + 0.25 * _yPos;
					xAvgSn = xAvgS1;
					yAvgSn = yAvgS1;
				}

				if (_qryAddress == 0x52)
				{
					numberOfBlocksS2 = _numberOfBlocks;
					xAvgS2 = 0.75 * xAvgS2 + 0.25 * _xPos;
					yAvgS2 = 0.75 * yAvgS2 + 0.25 * _yPos;
					xAvgSn = xAvgS2;
					yAvgSn = yAvgS2;
				}

				xDelta = abs(xAvgS1 - xAvgS2);
				yDelta = abs(yAvgS1 - yAvgS2);


				if (numberOfBlocksS1 > 0 && numberOfBlocksS2 > 0)
				{
					xAvgSn = (xAvgS1 + xAvgS2)/2;
					yAvgSn = (yAvgS1 + yAvgS2)/2;
				}

			} // end for (int i = 1; i < 6; i ++)


			// if no objects are found, wait 1 seconds, then start rotational scanning
			if (numberOfBlocksS1 == 0 &&	numberOfBlocksS2 == 0)
			{
				if (time1[T1] > 1000)
				{
					fwdRevPwr = 22;

					//stop motors
					motor[motorB] = 0;
					motor[motorC] = 0;

					//set numberOfBlocksFlag to 0
					numberOfBlocksFlag = 0;

					//reset x any y avg vars
					xAvgS1=0;
					yAvgS1=0;
					xAvgS2=0;
					yAvgS2=0;
					xAvgSn=0;
					yAvgSn=0;
					xDelta=0;
					yDelta=0;

					offset_xPos = 45; // set to default rotation speed for scanning
				}
			}

			// if an object is found, track it in roughly the center of the frame
			if (_numberOfBlocks > 0)
			{
				clearTimer(T1); // clear (reset) timer 1, which is used to detect how long since last object detected

				if (numberOfBlocksFlag == 0) //if transitioning from no objects found
				{
					//stop motors for transition
					motor[motorB] = 0;
					motor[motorC] = 0;
					numberOfBlocksFlag = 1; //set numberOfBlocksFlag to 1
				}

				fwdRevPwr = (100 - yAvgSn)/7 ; // fwd or Rev motion at speed based on object yPos
				offset_xPos = xAvgSn - 127; // 127 is ~255/2
			}

			scaled_xPos=scaleInput(offset_xPos, inputMax, outputMax);

			// set motor speed (pwr)
			motorBpwr = fwdRevPwr + (K * scaled_xPos);
			motorCpwr = fwdRevPwr - (K * scaled_xPos);

			// pwr up motors
			motor[motorB] = rangeLim(motorBpwr, topSpeedLim);
			motor[motorC] = rangeLim(motorCpwr, topSpeedLim);


			eraseDisplay();
			displayTextLine(0, "numberOfBlocks: %d", _numberOfBlocks);
			displayTextLine(1, "xAvgSn: %3.1f", xAvgSn);
			displayTextLine(2, "offset_xPos: %d", offset_xPos);
			displayTextLine(3, "scaled_xPos: %d", scaled_xPos);
			displayTextLine(4, "motorBpwr: %d", motorBpwr);
			displayTextLine(5, "motorCpwr: %d", motorCpwr);
			displayTextLine(6, "yAvgSn: %3.1f", yAvgSn);
			displayTextLine(7, "Sig: %d", _qryAddress);
			wait1Msec(50);


		} // end for (_qryAddress = 0x51; _qryAddress < 0x53; _qryAddress ++)

	} // while end

}
