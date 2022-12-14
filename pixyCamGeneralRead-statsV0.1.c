#pragma config(Sensor, S1,     PIXYC,          sensorI2CCustom)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//


/**

This Program will repeatedly execute the pixy general qry and gather some stats.

- list of sigs detected, up to 7
- count of detections for each sig

*/

#include "pixyCamV1-lib-RK1b.h"

task main()
{

	short _sigVal=0;
	ubyte _xPos=0;
	ubyte _yPos=0;
	ubyte _width=0;
	ubyte _height=0;
	ubyte _angle=0;

	short sig1=0;
	short sig2=-1;
	short sig3=-1;
	short sig4=-1;
	short sig5=-1;
	short sig6=-1;
	short sig7=-1;
	short sig8=-2;

	int sig1count=0;
	int sig2count=0;
	int sig3count=0;
	int sig4count=0;
	int sig5count=0;
	int sig6count=0;
	int sig7count=0;
	int sig8count=0;

	initializeI2CSensor(PIXYC, false);
	sleep(150);

	while (true)
	{
		eraseDisplay();

		while (!PIXYCreadGeneralQry(PIXYC, _sigVal, _xPos, _yPos, _width, _height))
		{
			//wait for read to return bool 'true'
			sleep(50);
		}

		while (!PIXYCreadAngleQry(PIXYC, _angle))
		{
			//wait for read to return bool 'true'
			sleep(50);
		}


		// capture stats
		for (int i=1; i<9; i++)
		{
			// set sig 'n' (2 thru 8) to _sigVal  (sig1 is reserved for no sig found)
			// increment count for appropriate sig
			switch (i)
			{
			case 1:
				if (_sigVal == 0)
				{
					sig1count++;
					i=9;
				}
				break;

			case 2:
				if (sig2 == -1)
				{
					sig2 = _sigVal;
					sig2count++;
					i=9;
				}
				else
				{
					if (_sigVal == sig2)
					{
						sig2count++;
						i=9;
					}
				}
				break;

			case 3:
				if (sig3 == -1)
				{
					sig3 = _sigVal;
					sig3count++;
					i=9;
				}
				else
				{
					if (_sigVal == sig3)
					{
						sig3count++;
						i=9;
					}
				}
				break;

			case 4:
				if (sig4 == -1)
				{
					sig4 = _sigVal;
					sig4count++;
					i=9;
				}
				else
				{
					if (_sigVal == sig4)
					{
						sig4count++;
						i=9;
					}
				}
				break;

			case 5:
				if (sig5 == -1)
				{
					sig5 = _sigVal;
					sig5count++;
					i=9;
				}
				else
				{
					if (_sigVal == sig5)
					{
						sig5count++;
						i=9;
					}
				}
				break;

			case 6:
				if (sig6 == -1)
				{
					sig6 = _sigVal;
					sig6count++;
					i=9;
				}
				else
				{
					if (_sigVal == sig6)
					{
						sig6count++;
						i=9;
					}
				}
				break;

			case 7:
				if (sig7 == -1)
				{
					sig7 = _sigVal;
					sig7count++;
					i=9;
				}
				else
				{
					if (_sigVal == sig7)
					{
						sig7count++;
						i=9;
					}
				}
				break;

			default:
				sig8count++;
			}

		}


		displayTextLine(0, "sig1: %d, %d", sig1, sig1count);
		displayTextLine(1, "sig2: %d, %d", sig2, sig2count);
		displayTextLine(2, "sig3: %d, %d", sig3, sig3count);
		displayTextLine(3, "sig4: %d, %d", sig4, sig4count);
		displayTextLine(4, "sig5: %d, %d", sig5, sig5count);
		displayTextLine(5, "sig6: %d, %d", sig6, sig6count);
		displayTextLine(6, "sig7: %d, %d", sig7, sig7count);
		displayTextLine(7, "sig8: %d, %d", sig8, sig8count);
		sleep(100);

		if (nNxtButtonPressed == 1) sleep(8000);

	}
}
