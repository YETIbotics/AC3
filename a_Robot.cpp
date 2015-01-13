#include "a_Robot.h"

// Constructors ////////////////////////////////////////////////////////////////

Robot::Robot()
{
	Usb.Init();
	
}

void Robot::Task(){
	Usb.Task();
}