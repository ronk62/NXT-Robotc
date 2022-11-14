#pragma config(Sensor, S1,     PIXYC,          sensorI2CCustom)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "pixyCamV1-lib-RK1b.h"


task main()
{

	ubyte _qryAddress = 0x51;   // The address of the Signature (51-57)
	ubyte _numberOfBlocks=0;
	ubyte _xPos=0;
	ubyte _yPos=0;
	ubyte _width=0;
	ubyte _height=0;

	int xPosArray[] =
	{
		0,
		0,
		0,
	};

	int execTArray[] =
	{
		0,
		0,
		0,
	};

	ubyte xPosThreshold=25;

	bool execStart=false;

	int i=0;

	int execT=0;
	int tTotal=0;
	int tMin=3000;
	int tMax=0;

	float dtAvg=0.0;
	float nBlocksAvg=0.0;


	initializeI2CSensor(PIXYC, false);
	sleep(150);

	while (true)
	{
		eraseDisplay();

		if (execStart==0) clearTimer(T1);

		while (!PIXYCreadSignatureQry(PIXYC, _qryAddress, _numberOfBlocks, _xPos, _yPos, _width, _height))
		{
			//wait for read to return bool 'true'
			sleep(50);
		}

		execT=time1[T1];
		tTotal=execT;
		dtAvg=(dtAvg+execT)/2;
		nBlocksAvg=(nBlocksAvg + _numberOfBlocks)/2;

		if (execT < tMin) tMin = execT;
		if (execT > tMax) tMax = execT;

		if (_xPos > xPosThreshold)
    {
      execStart=true;
      xPosArray[i] = _xPos;
      execTArray[i] = execT;
      xPosThreshold = xPosThreshold + 100;
      i++;
    }


		if (i==3 || nNxtButtonPressed == 1)
		{
			displayTextLine(0, "execT: %d", execT);
			displayTextLine(1, "tTotal: %d", tTotal);
			displayTextLine(2, "dtAvg: %4.2f", dtAvg);
			displayTextLine(3, "tMin: %d", tMin);
			displayTextLine(4, "tMax: %d", tMax);
			displayTextLine(5, "p1, t1: %d, %d", xPosArray[0], execTArray[0]);
			displayTextLine(6, "p2, t2: %d, %d", xPosArray[1], execTArray[1]);
			displayTextLine(7, "p3, t3: %d, %d", xPosArray[2], execTArray[2]);

			sleep(60000);

		}
	}
}
