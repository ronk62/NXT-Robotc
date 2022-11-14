task main()
{

	nMotorEncoder[motorA]=0;
	nMotorEncoderTarget[motorA]=90; //dont use negagtive target degrees
	motor[motorA] = -55;
	 while (nMotorRunState[motorA] != runStateIdle) //while the encoder wheel turns
  {
    // null
  }
	motor[motorA] = 0;
	wait1Msec(10000);

}
