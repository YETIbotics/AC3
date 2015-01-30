#include "a_Lift.h"

// Constructors ////////////////////////////////////////////////////////////////

Lift::Lift(Robot *p)
: liftPID(&liftCurPos, &liftPIDOut, &liftSetPoint, liftKP, liftKI, liftKD, DIRECT)
{
	robot = p;
}


void Lift::Task()
{
	liftPID.Compute();
	

	robot->LiftRightSpeed = ControllerSpeed;
	robot->LiftLeftSpeed = ControllerSpeed;
}

Lift::Right::Right()
: rightPID(&rightCurPos, &rightPIDOut, &rightSetPoint, rightKP, rightKI, rightKD, DIRECT)
{

}

void Lift::Right::Task()
{
	rightPID.Compute();
}