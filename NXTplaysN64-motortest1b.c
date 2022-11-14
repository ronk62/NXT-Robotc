void moveMotorA(int pwr, int target)
{
			nMotorEncoder[motorA]=0; //reset encoder
		nMotorEncoderTarget[motorA]=target;
		motor[motorA] = pwr;
		while(nMotorRunState[motorA]!=runStateIdle)// while Motor A is still running (hasn't reached target yet):

		{

			// do not continue

		}
		motor[motorA] = 0;
}


task main()
{
	//int pwr=0;
	//bFloatDuringInactiveMotorPWM=false; //coast the motor(s) when stopped

	//while (true)
	//{

	for(int i=0;i<11;i++) //iterate 10 times
	{

		moveMotorA(15,90); //pwr of 15 and target of 100 deg rotation
		wait1Msec(1000); //wait for 1 second before continuing

		moveMotorA(-15,-90); //pwr of 15 and target of 100 deg rotation
		wait1Msec(1000); //wait for 1 second before continuing

		moveMotorA(-15,-90); //pwr of 15 and target of 100 deg rotation
		wait1Msec(1000); //wait for 1 second before continuing

		moveMotorA(15,90); //pwr of 15 and target of 100 deg rotation
		wait1Msec(1000); //wait for 1 second before continuing

	}

	wait1Msec(120000); //wait for 2 mins before repeating
	//wait1Msec(8000); //for testing...wait for 8 secs before repeating
	alive(); //keep the brick from shutting down due to being idle

	//}
}
