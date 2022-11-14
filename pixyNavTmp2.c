#pragma config(Sensor, S1,     PIXYC,          sensorI2CCustom)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "pixyCamV1-lib-RK1b.h"


task main()
{

	//bool bFastI2C=true;
	ubyte _qryAddress = 0x51;   // The address of the Signature (51-57)
	ubyte _numberOfBlocks=0;
	ubyte _xPos=0;
	ubyte _yPos=0;
	ubyte _width=0;
	ubyte _height=0;

	ubyte xAvgS1=0;
	ubyte yAvgS1=0;
	ubyte xAvgS2=0;
	ubyte yAvgS2=0;
	ubyte xDelta=0;
	ubyte yDelta=0;


	initializeI2CSensor(PIXYC, false);
	sleep(150);

	while (true)
	{
		eraseDisplay();

		for (_qryAddress = 0x51; _qryAddress < 0x53; _qryAddress ++)

		{

			while (!PIXYCreadSignatureQry(PIXYC, _qryAddress, _numberOfBlocks, _xPos, _yPos, _width, _height))
			{
				//wait for read to return bool 'true'
				sleep(50);
			}

			if (_qryAddress == 0x51)
			{
				xAvgS1 = 0.75 * xAvgS1 + 0.25 * _xPos;
				yAvgS1 = 0.75 * yAvgS1 + 0.25 * _yPos;
			}
			else
			{
				xAvgS2 = 0.75 * xAvgS2 + 0.25 * _xPos;
				yAvgS2 = 0.75 * yAvgS2 + 0.25 * _yPos;
			}

			xDelta = abs(xAvgS1 - xAvgS2);
			yDelta = abs(yAvgS1 - yAvgS2);
		}


		displayTextLine(0, "xAvgS1: %d", xAvgS1);
		displayTextLine(1, "yAvgS1: %d", yAvgS1);
		displayTextLine(2, "xAvgS2: %d", xAvgS2);
		displayTextLine(3, "yAvgS2: %d", yAvgS2);
		displayTextLine(4, "xDelta: %d", xDelta);
		displayTextLine(5, "yDelta: %d", yDelta);

		sleep(100);

	}
}
