#include "a_Robot.h"

// Constructors ////////////////////////////////////////////////////////////////

Robot::Robot()
{
	Usb.Init();
	
}

void Robot::Read(){
	Usb.Task();
}

void Robot::Write(){

	//float asdf = controller->LeftJoystick;
}

void Robot::SetController(Controller *p)
{
	controller = p;
}