#include "a_Intake.h"

// Constructors ////////////////////////////////////////////////////////////////

Intake::Intake(Robot *p)
{
	robot = p;
}

void Intake::Task()
{
	if(ControllerSpeed == 1)
		robot->IntakeSpeed = 400;
	else if(ControllerSpeed == -1)
		robot->IntakeSpeed = -400;
	else
		robot->IntakeSpeed = 0;
	
}