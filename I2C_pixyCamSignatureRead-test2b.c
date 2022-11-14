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


	initializeI2CSensor(PIXYC, false);
	sleep(150);

	while (true)
	{
		eraseDisplay();

	  while (!PIXYCreadSignatureQry(PIXYC, _qryAddress, _numberOfBlocks, _xPos, _yPos, _width, _height))
			{
				//wait for read to return bool 'true'
				sleep(50);
			}

			displayTextLine(1, "numberOfBlocks: %d", _numberOfBlocks);
			displayTextLine(2, "xPos: %d", _xPos);
			displayTextLine(3, "yPos: %d", _yPos);
			displayTextLine(4, "width: %d", _width);
			displayTextLine(5, "height: %d", _height);

			sleep(100);

		}
}
