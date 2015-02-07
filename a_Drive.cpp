#include "a_Drive.h"

// Constructors ////////////////////////////////////////////////////////////////

Drive::Drive(Robot *p)
{
	robot = p;
}

void Drive::GoTo(double position)
{
	driveSetPoint = position;
}


void Drive::init()
{
	drivePID.init(&driveCurPos, &drivePIDOut, &driveSetPoint,driveKP,driveKI,driveKD, DIRECT);
	driveCurPos = robot->GetEncDriveLeft();
	drivePID.SetMode(AUTOMATIC);
	drivePID.SetOutputLimits(-400,400);
}


void Drive::Task()
{
	//Robot.mc1.SetSpeed(LeftDriveSpeed);

	driveCurPos = robot->GetEncDriveLeft();
	drivePID.Compute();



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
		driveSetPoint = driveCurPos;
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