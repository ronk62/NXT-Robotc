/**
* This is a quick program to test reaction wheels.

*/

task main()
{

	int i=0;
	int speedA=100;
	int speedB=0;

	bFloatDuringInactiveMotorPWM = true;

	motor[motorA] = speedA;
	motor[motorB] = speedB;

	for(i=0; i<120; i++)
	{
		wait1Msec(1000);
		eraseDisplay();
		displayTextLine(1, "RunStateA:%x", nMotorRunState[motorA]);
		displayTextLine(2, "RunStateB:%x", nMotorRunState[motorB]);
	}

	motor[motorA] = 0;
	motor[motorB] = 0;

	wait1Msec(2000);

	eraseDisplay();
	displayTextLine(5, "RunState motorA(hex):%x", nMotorRunState[motorA]);
	displayTextLine(6, "RunState motorB(hex):%x", nMotorRunState[motorB]);

	wait1Msec(30000);


}
