#include "a_Claw.h"

// Constructors ////////////////////////////////////////////////////////////////

Claw::Claw(Robot *p)
{
	robot = p;

	clawTimerId = timer.setInterval(500, turnOffClaw);
}

void Claw::turnOffClaw()
{
	robot->ClawPower = 0;
	timer.disable(clawTimerId);
}

void Claw::Task()
{
	if(ControllerClawPosition == 1)
		robot->ClawPower = 400;
	else
	{
		if(robot->ClawPower == 400)
		{
			robot->ClawPower = -400;
			timer.enable(clawTimerId);
		}
	}
	
	if(ControllerArmSpeed == 1)
		robot->ArmSpeed = 400;
	else if(ControllerArmSpeed == -1)
		robot->ArmSpeed = -400;
	else
		robot->ArmSpeed = 0;
}