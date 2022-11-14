#pragma config(Sensor, S1,     PIXYC,          sensorI2CCustom)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "pixyCamV1-lib-RK1a.h"


 task main()
{

	//bool bFastI2C=true;

	initializeI2CSensor(PIXYC, false);
	sleep(150);

	while (true)
	{
		eraseDisplay();

	  while (!PIXYCreadGeneralQry(PIXYC))
			{
				//wait for read to return bool 'true'
				sleep(50);
			}

			displayTextLine(1, "sigVal: %d", sigVal);
			displayTextLine(2, "xPos: %d", xPos);
			displayTextLine(3, "yPos: %d", yPos);
			displayTextLine(4, "width: %d", width);
			displayTextLine(5, "height: %d", height);

			if (xPos>0 && xPos<200) playImmediateTone(440, 25);

			sleep(100);

		}
}