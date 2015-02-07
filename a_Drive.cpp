#include "a_Drive.h"

// Constructors ////////////////////////////////////////////////////////////////

Drive::Drive(Robot *p)
{
	robot = p;
}

void Drive::DriveLeft(double position)
{
	driveLeftSetPoint += position;
}

void Drive::DriveRight(double position)
{
	driveRightSetPoint += position;
}

void Drive::Move(double position)
{
	driveLeftSetPoint += position;
	driveRightSetPoint += position;
}

void Drive::init()
{
	driveLeftPID.init(&driveLeftCurPos, &driveLeftPIDOut, &driveLeftSetPoint, driveLeftKP, driveLeftKI, driveLeftKD, DIRECT);
	driveLeftCurPos = robot->GetEncDriveLeft();
	driveLeftPID.SetMode(AUTOMATIC);
	driveLeftPID.SetOutputLimits(-400,400);

	driveRightPID.init(&driveRightCurPos, &driveRightPIDOut, &driveRightSetPoint, driveRightKP, driveRightKI, driveRightKD, DIRECT);
	driveRightCurPos = robot->GetEncDriveRight();
	driveRightPID.SetMode(AUTOMATIC);
	driveRightPID.SetOutputLimits(-400,400);
}


void Drive::Task()
{
	//Robot.mc1.SetSpeed(LeftDriveSpeed);

	driveLeftCurPos = robot->GetEncDriveLeft();
	driveLeftPID.Compute();

	driveRightCurPos = robot->GetEncDriveRight();
	driveRightPID.Compute();



	if(LeftControllerSpeed != 0 || RightControllerSpeed != 0)
	{
		float enc = robot->GetEncLiftRight();
		float k = 1;

		if(enc > 10)
		{
			if(enc > 30)
			{
				enc = 30;
			}

			k = ((1.0-((enc - 10)/20)) * 0.6) + 0.4; 

		}

		robot->DriveRightSpeed = RightControllerSpeed * k;
		robot->DriveLeftSpeed = LeftControllerSpeed * k;
		driveLeftSetPoint = driveLeftCurPos;
		driveRightSetPoint = driveRightCurPos;
	} 
	else
	{
		/*
		if(abs(driveSetPoint - driveCurPos) > drivePIDTolerence) 
		{
			robot->DriveLeftSpeed = drivePIDOut;
			robot->DriveRightSpeed = drivePIDOut;
		}
		else 
		{*/
			robot->DriveLeftSpeed = 0;
			robot->DriveRightSpeed = 0;
		//}
	}

	
}