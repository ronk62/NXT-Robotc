/**
* This is a quick program to test the behavior of nMotorEncoderTarget[]
* Findings are as follows...
* At minimum power ('1') there is an unexpected and unwanted jump of 26-27 degrees
* at the end of the cycle.
* - typical encoder increment per 100 mS t is 0-3, with an average of 0.9
* - peak encoder increment per 100 mS is 4, with the exception of the 27 deg jump
*
* At a power setting of 27 the unexpected/unwanted jump of 26-27 degrees blends-in to
* the normal increment per 100 mS.
* - typical encoder increment per 100 mS t is 26, with an average of 26.6
* - peak encoder increment per 100 mS at this power setting is 30
*
* At a power setting of 54 (2x the above) the unexpected/unwanted jump of 26-27 degrees
* again blends-in and is below normal increment per 100 mS.
* - typical encoder increment per 100 mS t is 54, with an average of 52.6
* - peak encoder increment per 100 mS at this power setting is 60
*/

task main()
{
	int i=1;
	int MotorAencoderValN=0;
	int peakMotorAencoderValN=0;
	int MotorAencoderValT=0;
	float MotorAencoderValAvgPerSec=0.0;

	nMotorEncoder[motorA]=0;

	nMotorEncoderTarget[motorA]=4000;
	motor[motorA] = 54;

	while(nMotorRunState[motorA] != runStateIdle) {
	//for (i=1; i<601; i++) {
		wait1Msec(100);
		MotorAencoderValN=nMotorEncoder[motorA];
		if (MotorAencoderValN>peakMotorAencoderValN) peakMotorAencoderValN=MotorAencoderValN;
		MotorAencoderValT=MotorAencoderValT+MotorAencoderValN;
		MotorAencoderValAvgPerSec=10*MotorAencoderValT/i;
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
