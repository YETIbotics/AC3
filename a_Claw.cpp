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

void Claw::Clamp()
{
	robot->ClawPower = -399;
	clamping = true;
}
void Claw::DeClamp()
{
	robot->ClawPower = 0;
	clamping = false;
}

void Claw::turnOffClaw()
{
	//Serial.println("turnOffClaw");
	if(!clamping)
	{
	robot->ClawPower = 0;
	timer.disable(clawTimerId);
}
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
//	Serial.println(armSetPoint);

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
	if(ControllerArmSpeed == 1) //RIGHT
	{
		//robot->ArmSpeed = 200;
		armSetPoint = 78;
	}
	else if(ControllerArmSpeed == -1) //LEFT
	{
		//robot->ArmSpeed = -200;
		armSetPoint = 48;
	}
	else if(ControllerArmSpeed == 2) //UP
	{
		//robot->ArmSpeed = -200;
		armSetPoint = 62;
	}
	else if(ControllerArmSpeed == -2) //DOWN
	{
		//robot->ArmSpeed = -200;
		armSetPoint = 0;
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
	
}