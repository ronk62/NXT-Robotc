task main()
{
	int pwr=0;
	bFloatDuringInactiveMotorPWM=true; //coast the motor(s) when stopped

	while (true)
	{
		nMotorEncoder[motorA]=0; //reset encoder
		for(int i=0;i<1500;i++) //iterate for 3000 ms
		{
			pwr=i/40;
			motor[motorA] = pwr;  //motor speed will ramp up to approx 37
			wait1Msec(2);  //run the motor for 3 secs
		}

		for(int i=1500;i>0;i--) //iterate for 3000 ms
		{
			pwr=i/40;
			motor[motorA] = pwr;  //motor speed will ramp down from 37 to 0
			wait1Msec(2);  //run the motor for 3 secs
		}
		motor[motorA] = 0; //motor will not ramp 'down' but will coast to stop
		wait1Msec(120000); //wait for 2 mins before repeating
		//wait1Msec(8000); //for testing...wait for 8 secs before repeating
		alive(); //keep the brick from shutting down due to being idle

	}
}
