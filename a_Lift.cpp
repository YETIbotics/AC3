#include "a_Lift.h"

// Constructors ////////////////////////////////////////////////////////////////

Lift::Lift(Robot *p)
{
	robot = p;
}

void Lift::Task()
{
	robot->LiftRightSpeed = ControllerSpeed;
	robot->LiftLeftSpeed = ControllerSpeed;
}