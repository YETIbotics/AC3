#include "a_Lift.h"

// Constructors ////////////////////////////////////////////////////////////////

Lift::Lift(Robot *p)
{
	robot = p;
}



void Lift::LiftTo(double position)
{
	liftSetPoint = position;
}

void Lift::LiftAdd(double position)
{
	liftSetPoint += position;
}



void Lift::init()
{
	liftPID.init(&liftCurPos, &liftPIDOut, &liftSetPoint,liftKP,liftKI,liftKD, REVERSE);
	liftCurPos = robot->GetEncLiftRight();
	liftPID.SetMode(AUTOMATIC);
	liftPID.SetOutputLimits(-400,400);
}

void Lift::Task()
{

	liftCurPos = robot->GetEncLiftRight();
	liftPID.Compute();

	if(ControllerSpeed != 0)
	{
		robot->LiftRightSpeed = ControllerSpeed;
		robot->LiftLeftSpeed = ControllerSpeed;
		liftSetPoint = liftCurPos;
	} 
	else
	{
		if(abs(liftSetPoint - liftCurPos) > liftPIDTolerence) 
		{
			robot->LiftLeftSpeed = liftPIDOut;
			robot->LiftRightSpeed = liftPIDOut;
		}
		else 
		{
			robot->LiftLeftSpeed = 0;
			robot->LiftRightSpeed = 0;
		}
	}

	
}

