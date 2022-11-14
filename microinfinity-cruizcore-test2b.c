#pragma config(Sensor, S1,     MICC,                sensorI2CCustom)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/**
* microinfinity-cruizcore.h provides an API for the MicroInfinity CruizCore XG1300L sensor.
* This program demonstrates how to use that API.
*
* Changelog:
* - 0.1: Initial release
*
* Credits:
* - Big thanks to MicroInfinity for providing me with the hardware necessary to write and test this.
*
* License: You may use this code as you wish, provided you give credit where it's due.
*
* THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 4.10 AND HIGHER

* Xander Soldaat (xander_at_botbench.com)
* 29 May 2011
* version 0.1
*/

#include "microinfinity-cruizcore.h"

short angleI = 0;
short rotI = 0;
short _x_accel = 0;
short _y_accel = 0;
short _z_accel = 0;

task updateHUD () {
	short x = 0;
	short y = 0;

	int t=120;
	playTone(440, 10);
	wait1Msec(t);
	clearSounds();
	playTone(440, 10);
	wait1Msec(t);
	clearSounds();
	playTone(220, 10);
	wait1Msec(t);
	clearSounds();
	playTone(220, 15);
	wait1Msec(t);
	clearSounds();

	while (true) {

		playTone(1000, 10);
		wait1Msec(t);
		clearSounds();

		eraseRect(4,50, 44,10);
		displayTextLine(2, "        H: %3d", angleI/100);
		displayTextLine(3, "        X: %3d", _x_accel/100);
		displayTextLine(4, "        Y: %3d", _y_accel/100);
		displayTextLine(5, "        Z: %3d", _z_accel/100);

		//displayTextLine(2, "Heading: %4.2f", angleI/100);
		//displayTextLine(3, "RoT:     %4.2f", rotI/100);
		//displayTextLine(5, "X:      %5.2f", _x_accel/100.0);
		//displayTextLine(6, "Y:      %5.2f", _y_accel/100.0);
		//displayTextLine(7, "Z:      %5.2f", _z_accel/100.0);

		drawCircle(84, 50, 4);

		drawCircle(4, 50, 40);
		x = (cosDegrees(-1 * (angleI/100 - 90)) * 20) + 24;
		y = (sinDegrees(-1 * (angleI/100 - 90)) * 20) + 30;
		drawLine(24, 30, x, y);
		eraseRect(0,0, 99, 8);
		drawRect(0,0, 99, 8);
		fillRect(50,0, (float)(rotI / 150)/100.0 *50 + 50, 8);
		sleep(1000);
	}
}

task main () {

	displayCenteredTextLine(0, "MicroInfinity");
	displayTextLine(1, "CruizCore XG1300L");
	displayCenteredTextLine(3, "Test 2");
	sleep(2000);
	eraseDisplay();

	// There are 3 ranges the Cruizcore XG1300L can measure in
	// up to 2G
	// up to 4G
	// up to 8G
	MICCsetRange8G(MICC);

	// Make sure you always reset the sensor at the beginning of your program
	// The robot needs to be completely stationary or your heading and gyro
	// data won't be accurate.
	MICCreset(MICC);

	// Start the task for displaying all the sensor data.
	startTask(updateHUD);

	displayTextLine(0, "CruizCore XG1300L");

	while (true) {

		// Read the relative heading from the sensor.
		angleI = MICCreadRelativeHeading(MICC);

		// Read the rate of turn
		rotI = MICCreadTurnRate(MICC);

		// Read the acceleration data from the sensor
		if (!MICCreadAccel(MICC, _x_accel, _y_accel, _z_accel)) {
			stopTask(updateHUD);
			sleep(100);
			eraseDisplay();
			displayTextLine(4, "ERROR!!");
			sleep(2000);
			stopAllTasks();
		}
		sleep(100);
	}
}