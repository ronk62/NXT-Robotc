task main()
{
  //
  // Firmware has been instrumented to trace the motor speed/PID regulation on a single motor. The output is
  // sent to the Debugger's "Debug Stream" window. By default, the trace messages are disabled. You should
  // be continuously connected  (via USB or Bluetooth) to the NXT to get the full trace output; otherwise the
  // internal buffers can overflow and some trace records can be lost.
  //
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
													8,       // nMinimumRampStartUpSpeed,
													 1,       // nRampStartupChangeRate,
													8,       // nMinimumRampDownSpeed,
													 1);      // nRampDownChangeRate)

	//
	// For testing we'll simply move the motor forward 900 encoder ticks and then move backwards 900 ticks.
  //
  while (true)
	{
	  nMotorEncoderTarget[motorA] = 450;
	  motor[motorA] = 1;

	  while (nMotorRunState[motorA] != runStateIdle)
	  {}

	  sleep(2000);

	  nMotorEncoderTarget[motorA] = 450;
	  motor[motorA] = -1;

	  while (nMotorRunState[motorA] != runStateIdle)
	  {}
	  break;
	}
}
