#pragma config(Sensor, S1,     PIXYC,          sensorI2CCustom)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "pixyCamV1-lib-RK1a.h"


 task main()
{

	//bool bFastI2C=true;

  ubyte ColorCodeByte1 = 0x0a;   // The LSB of color code (octal).
	ubyte ColorCodeByte2 = 0x00;   // The MSB of color code (octal).

	initializeI2CSensor(PIXYC, false);
	sleep(150);

	while (true)
	{
		eraseDisplay();

	  while (!PIXYCreadColorCodeQry(PIXYC, ColorCodeByte1, ColorCodeByte2))
			{
				//wait for read to return bool 'true'
				sleep(50);
			}

			displayTextLine(1, "numberOfBlocks: %d", numberOfBlocks);
			displayTextLine(2, "xPos: %d", xPos);
			displayTextLine(3, "yPos: %d", yPos);
			displayTextLine(4, "width: %d", width);
			displayTextLine(5, "height: %d", height);
			displayTextLine(6, "angle: %d", angle);

			sleep(100);

		}
}