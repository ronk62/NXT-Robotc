#pragma config(Sensor, S1,     PIXYC,          sensorI2CCustom)
#pragma config(Motor,  motorB,          LeftMotor,     tmotorNXT, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorC,          RightMotor,    tmotorNXT, PIDControl, driveRight, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/**
* This program will use the pixycam to track a color-coded object. The pixycam is
* mounted to a differential drive platform, which will move according to the pixycam values
*
* Revisions:
* 0.1  - initial version
* 0.2  - added area calculation, increased topSpeedLim and decreased pwrScaling values
* 0.2b - major rewrite, removing nSyncedMotors method in order to support fwd/rev movement
*        of the platform
* 0.3b - set fwdRevPwr based on area of object...use(P) proportional method
* 1.0b - will be the initial 'release' (when ready)
*
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
	ubyte _ColorCodeByte1 = 0x0a;   // The LSB of color code (octal)
	ubyte _ColorCodeByte2 = 0x00;   // The MSB of color code (octal)
	ubyte _numberOfBlocks=0;
	ubyte _xPos=0;
	int offset_xPos=0;
	int scaled_xPos=0;
	ubyte _yPos=0;
	ubyte _width=0;
	ubyte _height=0;
	ubyte _angle=0;
	int area=0; // had to use 'int', since ubyte tops out at 255

	int motorBpwr=0;
	int motorCpwr=0;
	int topSpeedLim=30;  // set this to a very low value for initial testing
	int outputMax=40;    // max motor power +/-40
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
		while (!PIXYCreadColorCodeQry(PIXYC, _ColorCodeByte1, _ColorCodeByte2, _numberOfBlocks, _xPos, _yPos, _width, _height, _angle))
		{
			//wait for read to return bool 'true'
			sleep(50);
		}

		area = _width * _height;
		fwdRevPwr = (700 - area)/40 ;        // fwd or Rev motion at speed based on object area
		//if (area < 400) fwdRevPwr = 22;    // move fwd (towards object)
		//if (area >= 2000) fwdRevPwr = -22; // move Rev (away from object)

		// if no objects are found, wait 1 seconds, then start rotational scanning
		if (_numberOfBlocks == 0)
		{
			if (time1[T1] > 1000)
			{
				//stop motors
				motor[motorB] = 0;
				motor[motorC] = 0;
				// rotate right = 0 (fwdRevPwr) +/- K * 7 (scaled_xPos)
				fwdRevPwr = 0;
				offset_xPos = 35; // set to default rotation speed for scanning
			}
		}

		// if an object is found, track it in roughly the center of the frame
		else
		{
			clearTimer(T1); // clear (reset) timer 1, which is used to detect how long since last object detected
			offset_xPos = _xPos - 125; // 125 is ~255/2
		}

		scaled_xPos=scaleInput(offset_xPos, inputMax, outputMax);

		// set motor speed (pwr)
		motorBpwr = fwdRevPwr + (K * scaled_xPos);
		motorCpwr = fwdRevPwr - (K * scaled_xPos);

		// pwr up motors
		motor[motorB] = rangeLim(motorBpwr, topSpeedLim);
		motor[motorC] = rangeLim(motorCpwr, topSpeedLim);

		eraseDisplay();
		displayTextLine(1, "numberOfBlocks: %d", _numberOfBlocks);
		displayTextLine(2, "raw_xPos: %d", _xPos);
		displayTextLine(3, "offset_xPos: %d", offset_xPos);
		displayTextLine(4, "scaled_xPos: %d", scaled_xPos);
		displayTextLine(5, "motorBpwr: %d", motorBpwr);
		displayTextLine(6, "motorCpwr: %d", motorCpwr);
		//displayTextLine(6, "RunState(hex):%x", nMotorRunState[motorB]);
		//displayTextLine(5, "width: %d", _width);
		//displayTextLine(6, "height: %d", _height);
		displayTextLine(7, "Area: %d", area);
		wait1Msec(50);


	} // while end

}
