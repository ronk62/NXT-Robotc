task main()
{

	// The statement below initializes the trace on 'motorA'.
	//
	nMotorBeingTrace = motorA;

	nMotorPIDSpeedCtrl[motorA] = mtrSpeedReg;

	//
	// Firmware has been enhanced to smoothly ramp up the speed regulation on a motor. And it will smoothly ramp
	// down the speed when using firmware to move to a target encoder position. The ramping parameters are configured
	// on a per motor basis with the folowing intrinsic function call.
	//
	// There are two parameters for ramping up and ramping down:
	//   1. The rate at which the speed is adjusted. An adjustment is made every 25 milliseconds.
	//   2. The initial starting speed to use when starting from stopped condition. This avoids the "dead band" that
	//      occurs at very low raw power levels where the motor is not moving. Note that a value of zero is a flag to
	//      indicate that speed ramping is not used. For "ramp down" the minimum speed is used to indicate the lowest
	//      speed that the motor should achieve.
	//
	// By default, these parameters are all zero indicating that speed ramping is disabled.
	//
	// NOTE: When moving to a target encoder position the firmware already had a PID based algorithm to move the
	//       motor to a smooth stop at the target encoder position. This algorithm "kicks in" when there are about
	//       50 to 100 encoder ticks left before reaching the target encoder position; the exact distance at which
	//       it kicks in is based on the current speed setting of the motor -- it is a longer distance for higher
	//       speeds. The "ramp down" function for this function is applied before this PID algorithm takes effect.
	//
	changeSpeedRampingParms(motorA,
	1,       // nMinimumRampStartUpSpeed,
	1,       // nRampStartupChangeRate,
	1,       // nMinimumRampDownSpeed,
	1);      // nRampDownChangeRate)

	//
	// For testing we'll simply move the motor forward 900 encoder ticks and then move backwards 900 ticks.
	//

	while (true)
	{
		bFloatDuringInactiveMotorPWM=true; //coast the motor(s) when stopped
		motor[motorA] = 45;  //motor speed will ramp to 45
		wait1Msec(1500);  //run the motor for 1.5 secs
		motor[motorA] = 0; //motor will not ramp 'down' but will coast to stop
		wait1Msec(120000); //wait for 2 mins before repeating
		alive(); //keep the brick from shutting down due to being idle

	}
}
