#include "a_Drive.h"

// Constructors ////////////////////////////////////////////////////////////////

Drive::Drive(Robot *p)
{
	robot = p;
}

void Drive::Task()
{
	Robot.mc1.SetSpeed(LeftDriveSpeed);
}