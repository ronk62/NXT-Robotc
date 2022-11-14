task main()
{
	int pwr=0;
	//bFloatDuringInactiveMotorPWM=false; //coast the motor(s) when stopped

	//while (true)
	//{

	for(int i=0;i<11;i++) //iterate 10 times
	{
		nMotorEncoder[motorA]=0; //reset encoder
		pwr=15;
		nMotorEncoderTarget[motorA]=100;
		motor[motorA] = pwr;
		while(nMotorRunState[motorA]!=runStateIdle)// while Motor A is still running (hasn't reached target yet):

		{

			// do not continue

		}
		motor[motorA] = 0;
		wait1Msec(2000); //wait for 2 seconds before continuing


		//reverse direction
		nMotorEncoder[motorA]=0; //reset encoder
		pwr=-15;
		nMotorEncoderTarget[motorA]=-100;
		motor[motorA] = pwr;
		while(nMotorRunState[motorA]!=runStateIdle)// while Motor A is still running (hasn't reached target yet):

		{

			// do not continue

		}
		motor[motorA] = 0;
		wait1Msec(2000); //wait for 2 seconds before continuing

	}

	wait1Msec(120000); //wait for 2 mins before repeating
	//wait1Msec(8000); //for testing...wait for 8 secs before repeating
	alive(); //keep the brick from shutting down due to being idle

	//}
}
