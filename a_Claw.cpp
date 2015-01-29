#include "a_Claw.h"

// Constructors ////////////////////////////////////////////////////////////////

Claw::Claw(Robot *p)
{
	robot = p;

	
}

void Claw::turnOffClaw()
{
	Serial.println("turnOffClaw");
	robot->ClawPower = 0;
	//timer.disable(clawTimerId);
}

void Claw::init()
{
	//timer_delegate del = MakeDelegate(this, &Claw::turnOffClaw);
	//clawTimerId = timer.setInterval(500, del);
}

void Claw::Task()
{
	

	if(ControllerClawPosition == 1)
		robot->ClawPower = -400;
	else
	{
		if(robot->ClawPower == -400)
		{
			robot->ClawPower = 400;
			delatch = millis() + 500;
			//timer.enable(clawTimerId);
		}
	}

	if(delatch != 0 && millis() > delatch)
	{
		turnOffClaw();
		delatch = 0;
	}

	//Serial.println(ControllerArmSpeed);
	if(ControllerArmSpeed == 1)
		robot->ArmSpeed = 200;
	else if(ControllerArmSpeed == -1)
		robot->ArmSpeed = -200;
	else
		robot->ArmSpeed = 0;
}