/**
* This is a quick program to test reaction wheels.

*/

task main()
{
	int speedA=0;
	int speedB=0;
	bool x = 0;
	bool updateSpeed = 0;

	nMotorEncoder[motorA]=0;
	nMotorEncoder[motorB]=0;

  eraseDisplay();
  displayBigTextLine(3, "Default motor speeds A=30, B=0");
  displayBigTextLine(5, "Update Motor Speeds?");
  displayBigTextLine(6, "(LeftButton=no, RightButton=yes)");

while (x=0)
{
		// The nNxtButtonPressed variable contains
	  // the name of the button that is being pressed.
		// Read this and display its value on the screen.
		switch(nNxtButtonPressed)
		{
			case kNoButton: 		displayCenteredBigTextLine(4, "None");
			case kLeftButton: 	displayCenteredBigTextLine(4, "Left"); x=1; 	break;
			case kRightButton: 	displayCenteredBigTextLine(4, "Right"); updateSpeed = 1;
		}

	  while (nNxtButtonPressed=updateSpeed)
     {
  	   DisplayBigTextLine(1, "Select L Arrow to cycle thru motorA speeds");
  	   DisplayBigTextLine(2, "Select R Arrow to cycle thru motorB speeds");
  	   DisplayBigTextLine(3, "Select Enter to proceed");

		switch(nNxtButtonPressed)
		{
			case kNoButton: 		displayCenteredBigTextLine(5, "None");
			case kLeftButton: 	displayCenteredBigTextLine(5, "Left"); x=1; 	break;
			case kRightButton: 	displayCenteredBigTextLine(5, "Right"); updateAspeed = 1;
			case kEnterButton: 	displayCenteredBigTextLine(5, "Enter");updateSpeed = 0; x=0; break;
		}

		// Wait 20 ms, this gives us 50 readings per second
		sleep(20);
	}




  while (x=0)
	{
		// The nNxtButtonPressed variable contains
	  // the name of the button that is being pressed.
		// Read this and display its value on the screen.
		switch(nNxtButtonPressed)
		{
			case kNoButton: 		displayCenteredBigTextLine(4, "None"); 	break;
			case kLeftButton: 	displayCenteredBigTextLine(4, "Left"); 	break;
			case kEnterButton: 	displayCenteredBigTextLine(4, "Enter"); break;
			case kRightButton: 	displayCenteredBigTextLine(4, "Right"); break;
		}
		// Wait 20 ms, this gives us 50 readings per second
		sleep(20);
	}
}




	while (true) {
		wait1Msec(1000);
		eraseDisplay();
		displayTextLine(1, "RunState motorA(hex):%x", nMotorRunState[motorA]);
		displayTextLine(2, "RunState motorB(hex):%x", nMotorRunState[motorB]);
	}

	motor[motorA] = 0;
	motor[motorB] = 0;

	wait1Msec(2000);

	eraseDisplay();
	displayTextLine(5, "RunState motorA(hex):%x", nMotorRunState[motorA]);
	displayTextLine(6, "RunState motorB(hex):%x", nMotorRunState[motorB]);

	wait1Msec(30000);


}
