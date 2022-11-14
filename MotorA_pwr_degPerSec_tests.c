/**
* This is a quick program to test the speed of the motor (in degrees per second) at
* various speed settings.

* Findings are that a motor power of '1' results in about 9-10 deg per second.
* And a power of '36' results in about 360 degrees per second.
* So the approximate rule is that power * 10 = degPerSec.
*/

task main()
{
	int i=0;
	int MotorAencoderValN=0;
	int MotorAencoderValT=0;
	float MotorAencoderValAvgPerSec=0.0;

	nMotorEncoder[motorA]=0;

	motor[motorA] = 1;

	for (i=1; i<121; i++) {
		wait1Msec(1000);
		MotorAencoderValN=nMotorEncoder[motorA];
		MotorAencoderValT=MotorAencoderValT+MotorAencoderValN;
		MotorAencoderValAvgPerSec=MotorAencoderValT/i;
		eraseDisplay();
		displayTextLine(1, "i: %d", i);
		displayTextLine(2, "Val: %d", MotorAencoderValN);
		displayTextLine(3, "ValT: %d", MotorAencoderValT);
		displayTextLine(4, "ValAvg/Sec: %d", MotorAencoderValAvgPerSec);
		displayTextLine(6, "RunState(hex):%x", nMotorRunState[motorA]);

		nMotorEncoder[motorA]=0;

	}

	motor[motorA] = 0;
	wait1Msec(2000);
	displayTextLine(6, "RunState(hex):%x", nMotorRunState[motorA]);

	wait1Msec(30000);


}
