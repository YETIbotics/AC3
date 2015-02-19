#include "a_Drive.h"

// Constructors ////////////////////////////////////////////////////////////////

Drive::Drive(Robot *p)
{
	robot = p;
}

void Drive::SetTorqueLimit(int val)
{
	robot->TorqueLimitDrive = val;
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
	driveLeftPID.init(&driveLeftCurPos, &driveLeftPIDOut, &driveLeftCorrSetPoint, driveLeftKP, driveLeftKI, driveLeftKD, DIRECT);
	driveLeftCurPos = robot->GetEncDriveLeft();
	driveLeftPID.SetMode(AUTOMATIC);
	driveLeftPID.SetOutputLimits(-400,400);

	driveRightPID.init(&driveRightCurPos, &driveRightPIDOut, &driveRightCorrSetPoint, driveRightKP, driveRightKI, driveRightKD, DIRECT);
	driveRightCurPos = robot->GetEncDriveRight();
	driveRightPID.SetMode(AUTOMATIC);
	driveRightPID.SetOutputLimits(-400,400);
}


void Drive::Task()
{
	robot->DriveLeftSpeed;
	robot->DriveRightSpeed;

	int enc = robot->GetEncLiftRight();
		
	if(enc <= 1)
	{
		correctionVal = 0;
	}
	else if (enc <= 5)
	{
		// 4 -- 15
		correctionVal = 15;
	}
	else if (enc <= 10)
	{
		//5 -- -5
		correctionVal = 10;
	}
	else if (enc <= 15)
	{
		correctionVal = -7;
	}
	else if (enc <= 20)
	{
		correctionVal = -27;
	}
	else if (enc <= 25)
	{
		correctionVal = -43;
	}
	else if (enc <= 30)
	{
		correctionVal = -52;
	}
	else if (enc <= 35)
	{
		correctionVal = -51;
	}
	else if (enc <= 100)
	{
		correctionVal = -34;
	}


/*	int enc = robot->GetEncLiftRight();
		
	if (enc = 1)
	{
		correctionVal = 0;
	}
	else if (enc = 2)
	{
		correctionVal = 4;
	}
	else if (enc = 3)
	{
		correctionVal = 8;
	}
	else if (enc = 4)
	{
		correctionVal = 12;
	}
	else if (enc = 5)
	{
		// 4 -- 15
		correctionVal = 15;
	}
	else if (enc = 6)
	{
		correctionVal = 14;
	}
	else if (enc = 7)
	{
		correctionVal = 13;
	}
	else if (enc = 8)
	{
		correctionVal = 12;
	}
	else if (enc = 9)
	{
		correctionVal = 11;
	}
	else if (enc = 10)
	{
		//5 -- -5
		correctionVal = 10;
	}
	else if (enc = 11)
	{
		correctionVal = 6;
	}
	else if (enc = 12)
	{
		correctionVal = 2;
	}
	else if (enc = 13)
	{
		correctionVal = -1;
	}
	else if (enc = 14)
	{
		correctionVal = -4;
	}
	else if (enc = 15)
	{
		correctionVal = -7;
	}
	else if (enc = 16)
	{
		correctionVal = -11;
	}
	else if (enc = 17)
	{
		correctionVal = -15
	}
	else if (enc = 18)
	{
		correctionVal = -19;
	}
	else if (enc = 19)
	{
		correctionVal = -23;
	}
	else if (enc = 20)
	{
		correctionVal = -27;
	}
	else if (enc = 21)
	{
		correctionVal = -30;
	}
	else if (enc = 22)
	{
		correctionVal = -33;
	}
	else if (enc = 23)
	{
		correctionVal = -36;
	}
	else if (enc = 24)
	{
		correctionVal = -40;
	}
	else if (enc = 25)
	{
		correctionVal = -43;
	}
	else if (enc = 26)
	{
		correctionVal = -45;
	}
	else if (enc = 27)
	{
		correctionVal = -47;
	}
	else if (enc = 28)
	{
		correctionVal = -49;
	}
	else if (enc = 29)
	{
		correctionVal = -51;
	}
	else if (enc = 30)
	{
		correctionVal = -52;
	}
	else if (enc <= 35)
	{
		correctionVal = -51;
	}
	else if (enc <= 100)
	{
		correctionVal = -34;
	}
*/

	driveLeftCorrSetPoint = driveLeftSetPoint + correctionVal;
	driveLeftCurPos = robot->GetEncDriveLeft();
	driveLeftPID.Compute();

	driveRightCorrSetPoint = driveRightSetPoint + correctionVal;
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

			k = ((1.0-((enc - 10)/30)) * 0.6) + 0.4; 

		}

		robot->DriveRightSpeed = RightControllerSpeed * k;
		robot->DriveLeftSpeed = LeftControllerSpeed * k;
		driveLeftSetPoint = driveLeftCurPos - correctionVal;
		driveRightSetPoint = driveRightCurPos - correctionVal; 
	} 
	else
	{
		
		if(abs(driveLeftSetPoint - driveLeftCurPos) > driveLeftPIDTolerence || abs(driveRightSetPoint - driveRightCurPos) > driveRightPIDTolerence) 
		{
			robot->DriveLeftSpeed = driveLeftPIDOut;
			robot->DriveRightSpeed = driveRightPIDOut;
		}
		else 
		{
			robot->DriveLeftSpeed = 0;
			robot->DriveRightSpeed = 0;
		}
		
	}



	
}