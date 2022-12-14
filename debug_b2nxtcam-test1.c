#pragma config(Sensor, S1,     cam,                 sensorI2CCustomFastSkipStates)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/**
* mindsensors-nxtcam.h provides an API for the Mindsensors NXTCam.  This program
* demonstrates how to use that API.
*
* Changelog:
* - 0.1: Initial release
* - 0.2: More comments
*
* Credits:
* - Gordon Wyeth for writing the original driver and cam_display program
*
* License: You may use this code as you wish, provided you give credit where it's due.
*
* THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 4.10 AND HIGHER

* Xander Soldaat (xander_at_botbench.com)
* 28 November 2009
* version 0.2
*/

#define DEBUG_COMMON_H

#include "mindsensors-nxtcam.h"

int tone=440;
int dur=25;

int rangeLim(int val, int lim) //((abs(val)-abs(val-lim))/2)+lim/2 ...returns vals in range of 0-> lim
{
	val = ((abs(val)-abs(val-lim))/2)+lim/2;
	return val;
}

// short xscale(short x) - Scales x values from camera coordinates to screen coordinates.
short xscale(short x) {
	return ((x - 12) * 99) / 175;
}

// short yscale(short y) - Scales y values from camera coordinates to screen coordinates.
short yscale(short y) {
	return ((143 - y) * 63) / 143;
}

/*
Main task
*/
task main () {
	blob_array _blobs;
	// combine all colliding blobs into one
	bool _condensed = true;

	//blob_array _blobs;
	short _l, _t, _r, _b;
	short _nblobs;
	eraseDisplay();


	// Initialise the camera
	NXTCAMinit(cam);
	while(true) {
		eraseDisplay();
		// Fetch all the blobs, have the driver combine all
		// the colliding blobs.
		_nblobs = NXTCAMgetBlobs(cam, _blobs, _condensed);
		for (short i = 0; i < _nblobs; i++) {
			// Draw the scaled blobs
			_l = xscale(_blobs[i].x1);
			_t = yscale(_blobs[i].y1);
			_r = xscale(_blobs[i].x2);
			_b = yscale(_blobs[i].y2);
			fillRect(_l, _t, _r, _b);

			// beep if an object is found. Derive the upper x, y coords and use those
			// to set beep tone and repetition rate
			if (_nblobs>0)
			{
				tone=440+rangeLim(_t*3, 880);
				dur=rangeLim(_l*3, 400);
				playImmediateTone(tone,10);
				wait1Msec(100+dur);
			}
		}
		displayTextLine(1, "%d", _nblobs);
		sleep(100);
	}
}
