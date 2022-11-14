#pragma config(Sensor, S1,     MICC,           sensorI2CCustom)
#pragma config(Sensor, S2,     HTcompass,      sensorI2CHiTechnicCompass)
#pragma config(Sensor, S3,     HTgyro,         sensorI2CHiTechnicGyro)
#pragma config(Motor,  motorB,          LeftMotor,     tmotorNXT, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorC,          RightMotor,    tmotorNXT, PIDControl, driveRight, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/**
* microinfinity-cruizcore.h provides an API for the MicroInfinity CruizCore XG1300L sensor.
*/

#include "microinfinity-cruizcore.h"

int rangeLim(int val, int lim) //(abs(val+lim)-abs(val-lim))/2 ...returns vals in range of +/- lim
{
	val = (abs(val+lim)-abs(val-lim))/2;
	return val;
}

task main ()
{
	short angleI = 0;
	float angleF = 0.0;
	float errF = 0.0;
	float errFsum = 0.0;
	int pwr = 0;
	float minF = 0.0;
	float maxF = 0.0;

	displayCenteredTextLine(0, "MicroInfinity");
	displayTextLine(1, "CruizCore XG1300L");
	sleep(2000);
	eraseDisplay();

	// There are 3 ranges the Cruizcore XG1300L can measure in
	// up to 2G
	// up to 4G
	// up to 8G
	MICCsetRange8G(MICC);


	for (int i=0; i<10; i++)
	{
		// Make sure you always reset the sensor at the beginning of your program
		// The robot needs to be completely stationary or your heading and gyro
		// data won't be accurate.
		displayCenteredTextLine(0, "straight iter (i) %d",i);
		MICCreset(MICC);
		wait1Msec(3000);

		nMotorEncoder[LeftMotor]=0;
		nMotorEncoder[RightMotor]=0;
		wait1Msec(1);
		nSyncedMotors = synchBC; //LeftMotor is the master, RightMotor is the slave
		wait1Msec(1);
		nSyncedTurnRatio = 100;
		motor[LeftMotor] = 30; //turn LeftMotor on, which controls RightMotor at the same power
		wait1Msec(200);

		while (nMotorEncoder[LeftMotor]<360*3)
		{
			//writeDebugStreamLine("First While (nPgmTime,nSyncedTurnRatio,angleF,minF,maxF), %d, %d, %4.2f, %4.2f, %4.2f", nPgmTime,nSyncedTurnRatio,angleF,minF,maxF);
			eraseDisplay();

			// Read the relative heading from the sensor.
			angleI = MICCreadRelativeHeading(MICC);
			angleF = angleI / 100.0;
			if (angleF<minF) minF=angleF;
			if (angleF>maxF) maxF=angleF;

			displayCenteredTextLine(1, "LeftMotor %d",nMotorEncoder[LeftMotor]);
			displayCenteredTextLine(2, "RightMotor %d",nMotorEncoder[RightMotor]);
			displayCenteredTextLine(5, "%4.2f, %4.2f", minF, maxF);
			displayCenteredTextLine(6, "TrnRatio  %d", nSyncedTurnRatio);
			displayCenteredTextLine(7, "Heading: %4.2f", angleF);

			if(angleF<0)
			{
				//writeDebugStreamLine("if angleF<0 (nPgmTime,nSyncedTurnRatio,angleF,minF,maxF), %d, %d, %4.2f, %4.2f, %4.2f", nPgmTime,nSyncedTurnRatio,angleF,minF,maxF);
				nSyncedMotors = synchBC; //LeftMotor is the master, RightMotor is the slave
				nSyncedTurnRatio = 100+angleF*1;
				wait1Msec(1);
				motor[LeftMotor] = 30; //turn LeftMotor on, which controls RightMotor at the same power
				wait1Msec(1);
			}
			if(angleF>1)
			{
				//writeDebugStreamLine("if angleF>1 (nPgmTime,nSyncedTurnRatio,angleF,minF,maxF), %d, %d, %4.2f, %4.2f, %4.2f", nPgmTime,nSyncedTurnRatio,angleF,minF,maxF);
				nSyncedMotors = synchCB; //RightMotor is the master, LeftMotor is the slave
				nSyncedTurnRatio = 100-angleF*1;
				wait1Msec(1);
				motor[RightMotor] = 30; //turn RightMotor on, which controls LeftMotor at the same power
				wait1Msec(1);
			}
			wait1Msec(100);
		} //while end

		motor[LeftMotor]=0; //stop motors and wait for robot to settle
		motor[RightMotor]=0;
		writeDebugStreamLine("drive straight complete (nPgmTime,angleF,i), %d, %4.2f, %d", nPgmTime,angleF,i);
		wait1Msec(2000);

		//setup for 90 deg turn
		MICCreset(MICC);
		wait1Msec(3000);
		eraseDisplay();
		displayCenteredTextLine(0, "turn iter %d",i);
		angleF =0.0;
		nSyncedMotors = synchNone; //unSync motors
		nMotorEncoder[LeftMotor]=0;
		nMotorEncoder[RightMotor]=0;
		wait1Msec(1);

		//make the turn
		while (angleF<90)
		{
			angleI = MICCreadRelativeHeading(MICC);
			angleF = angleI / 100.0;
			motor[LeftMotor]=15;
		}
		motor[LeftMotor] = 0; //stop the turn
		wait1Msec(2000);

		motor[LeftMotor]=0; //stop motors and wait for robot to settle
		motor[RightMotor]=0;
		wait1Msec(2000);
		angleI = MICCreadRelativeHeading(MICC);
		angleF = angleI / 100.0;
		displayCenteredTextLine(7, "Heading: %4.2f", angleF);
		writeDebugStreamLine("90 deg turn complete (nPgmTime,angleF,i), %d, %4.2f, %d", nPgmTime,angleF,i);
		wait1Msec(5000);

	} //for end

	motor[LeftMotor]=0;
	motor[RightMotor]=0;
	displayCenteredTextLine(2, "actions complete");
	wait1Msec(90000);
}
