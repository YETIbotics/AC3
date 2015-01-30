#include "a_Claw.h"

// Constructors ////////////////////////////////////////////////////////////////

Claw::Claw(Robot *p)
: armPID(&armCurPos, &armPIDOut, &armSetPoint,armKP,armKI,armKD, DIRECT)
{
	robot = p;

	
}

void Claw::turnOffClaw()
{
	Serial.println("turnOffClaw");
	robot->ClawPower = 0;
	timer.disable(clawTimerId);
}

void Claw::init()
{
	timer_delegate del = MakeDelegate(this, &Claw::turnOffClaw);
	clawTimerId = timer.setInterval(500, del);
}

void Claw::Task()
{
	timer.run();

	armPID.Compute();

	if(ControllerClawPosition == 1)
		robot->ClawPower = -400;
	else
	{
		if(robot->ClawPower == -400)
		{
			robot->ClawPower = 400;
			timer.enable(clawTimerId);
		}
	}
	//Serial.println(ControllerArmSpeed);
	if(ControllerArmSpeed == 1)
		robot->ArmSpeed = 200;
	else if(ControllerArmSpeed == -1)
		robot->ArmSpeed = -200;
	else
		robot->ArmSpeed = 0;
}