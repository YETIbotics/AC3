#include "a_Lift.h"

// Constructors ////////////////////////////////////////////////////////////////

Lift::Lift(Robot *p)
: liftPID(&liftCurPos, &liftPIDOut, &liftSetPoint, liftKP, liftKI, liftKD, DIRECT)
{
	robot = p;
}

Right::Right(Robot *p)
: rightPID(&rightCurPos, &rightPIDOut, &rightSetPoint, rightKP, rightKI, rightKD, DIRECT)
{
	robot = p;
}

void Lift::Task()
{
	liftPID.Compute();
	

	robot->LiftRightSpeed = ControllerSpeed;
	robot->LiftLeftSpeed = ControllerSpeed;
}

void Right::Task()
{
	rightPID.Compute();
}