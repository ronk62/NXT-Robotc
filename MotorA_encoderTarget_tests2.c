/**
* This is a quick program to test the behavior of nMotorEncoderTarget[] with ramp up/down
* Findings are as follows... (note that wait has been reduced to 10 mS for these tests)
* At minimum power ('1') there is an unexpected and unwanted jump of 3-4 degrees
* at the end of the cycle.
* - typical encoder increment per 10 mS t is 0-1, with an average of 1.1
* - peak encoder increment per 10 mS is 1, with the exception of the 4 deg jump
*
* At a power setting of 27 the unexpected/unwanted jump of 3-4 degrees blends-in to
* the normal increment per 10 mS when using the stock ramp settings.
* - typical encoder increment per 10 mS t is 2-3, with an average of 2.7
* - peak encoder increment per 10 mS at this power setting is 4
*
* At a power setting of 54 (2x the above) the unexpected/unwanted jump of 3-4 degrees
* again blends-in and is below normal increment per 10 mS.
* - typical encoder increment per 10 mS is 5-6, with an average of 5.26
* - peak encoder increment per 10 mS at this power setting is 7
*/

task main()
{
	int i=1;
	int MotorAencoderValN=0;
	int peakMotorAencoderValN=0;
	int MotorAencoderValT=0;
	float MotorAencoderValAvgPerSec=0.0;

	nMotorBeingTrace = motorA;
	nMotorPIDSpeedCtrl[motorA] = mtrSpeedReg;


	/** stock settings...
	*  changeSpeedRampingParms(motorA,
	*												15,       // nMinimumRampStartUpSpeed,
	*												 5,       // nRampStartupChangeRate,
	*												15,       // nMinimumRampDownSpeed,
	*												 5);      // nRampDownChangeRate)
	*
  */

	changeSpeedRampingParms(motorA,
	1,       // nMinimumRampStartUpSpeed,
	1,       // nRampStartupChangeRate,
	10,       // nMinimumRampDownSpeed,
	2);      // nRampDownChangeRate)

	nMotorEncoder[motorA]=0;

	nMotorEncoderTarget[motorA]=4000;
	motor[motorA] = 27;



	while(nMotorRunState[motorA] != runStateIdle) {
		//for (i=1; i<601; i++) {
		wait1Msec(10);
		MotorAencoderValN=nMotorEncoder[motorA];
		if (MotorAencoderValN>peakMotorAencoderValN) peakMotorAencoderValN=MotorAencoderValN;
		MotorAencoderValT=MotorAencoderValT+MotorAencoderValN;
		MotorAencoderValAvgPerSec=100*MotorAencoderValT/i;
		eraseDisplay();
		displayTextLine(1, "i: %d", i);
		displayTextLine(2, "Val: %d", MotorAencoderValN);
		displayTextLine(3, "ValPeak: %d", peakMotorAencoderValN);
		displayTextLine(4, "ValT: %d", MotorAencoderValT);
		displayTextLine(5, "ValAvg/Sec: %d", MotorAencoderValAvgPerSec);
		displayTextLine(6, "RunState(hex):%x", nMotorRunState[motorA]);

		nMotorEncoder[motorA]=0;
		i++;

	}

	motor[motorA] = 0;

	wait1Msec(2000);
	displayTextLine(6, "RunState(hex):%x", nMotorRunState[motorA]);

	while(1) wait1Msec(3000);

}
