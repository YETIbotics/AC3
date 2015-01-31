#include "a_Claw.h"

// Constructors ////////////////////////////////////////////////////////////////

Claw::Claw(Robot *p)
{
	robot = p;

	
}

void Claw::GoTo(double position)
{
	armSetPoint = position;
}

void Claw::turnOffClaw()
{
	//Serial.println("turnOffClaw");
	robot->ClawPower = 0;
	timer.disable(clawTimerId);
}

void Claw::init()
{
	timer_delegate del = MakeDelegate(this, &Claw::turnOffClaw);
	clawTimerId = timer.setInterval(500, del);

	armPID.init(&armCurPos, &armPIDOut, &armSetPoint,armKP,armKI,armKD, DIRECT);
	armCurPos = robot->GetEncClaw();
	armPID.SetMode(AUTOMATIC);
	armPID.SetOutputLimits(-400,400);
}

void Claw::Task()
{
	timer.run();

	armCurPos = robot->GetEncClaw();
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
	{
		//robot->ArmSpeed = 200;
		armSetPoint = armSetPoint + 1;
	}
	else if(ControllerArmSpeed == -1)
	{
		//robot->ArmSpeed = -200;
		armSetPoint = armSetPoint - 1;
	}
	else
	{	
		
		//Serial.println(armPIDOut);
	}
	if(abs(armSetPoint - armCurPos) > armPIDTolerence)
	{
		robot->ArmSpeed = armPIDOut;
	}
	else
	{
		robot->ArmSpeed = 0;
	}
	if(ControllerYPress == 1)
	{
		armSetPoint = armSetPoint + 1;
	}
}