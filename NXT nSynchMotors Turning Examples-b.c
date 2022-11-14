//                  Synchronized Motor Turning Examples
//
//  There are three programs below.  The first program sychronizes motors B and C
//  where motor B is the master and motor C is the slave.(Lines 18-29) This program
//  uses timing to determine how long the robot will turn.

//  The second program(lines 33-50), which is commented out, but can be uncommented
//  and run uses a combination of synchronized motors and encoders to accurately
//  turn.
//
//  The third program (lines 54-75  )uses a combination of the LEGO encoders, the
//  nMotorEncoderTarget function and synchronized motors to accurately turn.

//  The programs use timing to determine how far the robot turns.
//
//  You can use and modify any of these programs to fit your own code

task main()
{

  nSyncedMotors = synchBC; //motor B is the master, motor C is the slave
  nSyncedTurnRatio = -100; //motors move in opposite directions of each other

  motor[motorB] = 30; //turn motor B on, which controls motor C at 30% power
  wait1Msec(500);  // wait for one half second

  motor[motorB] = 0;  // turn the motors off.

}

//the program below uses feedback from encoders to determine how much the robot turns.

/*task main()
{

  nSyncedMotors = synchBC; //motor B is the master, motor C is the slave
  nSyncedTurnRatio = -100; //motors move in opposite directions of one another

  nMotorEncoder[motorB] = 0;

  while(nMotorEncoder[motorB] < 360) //motor B turns one full wheel revolution
  {
  motor[motorB] = 30; //turn motor B on, which controls motor C at 30% power
  }

  motor[motorB] = 0;  // turn the motors off.

  wait1Msec(3000);

}*/

//the program below uses the nMotorTargetEncoder function with synchronized motors

/*task main()
{

  nSyncedMotors = synchBC; //motor B is the master, motor C is the slave
  nSyncedTurnRatio = -100; //motors move in opposite directions of one another

  nMotorEncoder[motorB] = 0; // clears the value of motorB's encoder

  nMotorEncoderTarget[motorB] = 360; // sets a target of 360 degrees

  motor[motorB] = 30; //turns the motor on at 30% power

  while(nMotorRunState[motorB] != runStateIdle) //while motorB is not in an idle state
  {
    //continue to power motorB until the motor nMotorEncoderTarget position is reached
  }

  motor[motorB] = 0;  // turn the motors off.

  wait1Msec(3000);

}*/
