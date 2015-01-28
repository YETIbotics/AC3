#include "a_Intake.h"

// Constructors ////////////////////////////////////////////////////////////////

Intake::Intake(Robot *p)
{
	robot = p;
}

void Intake::Task()
{
	robot->IntakeSpeed = ControllerSpeed;
}