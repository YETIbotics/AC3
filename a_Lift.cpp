#include "a_Lift.h"

// Constructors ////////////////////////////////////////////////////////////////

Lift::Lift(Robot *p)
: liftPID(&liftCurPos, &liftPIDOut, &liftSetPoint, liftKP,liftKI,liftKD, DIRECT)
: rightPID(&rightCurPos, &rightPIDOut, &rightSetPoint, rightKP, rightKI, rightKD, DIRECT)
{
	robot = p;
}

void Lift::Task()
{
		liftPID.Compute();
		rightPID.Comute();

	robot->LiftRightSpeed = ControllerSpeed;
	robot->LiftLeftSpeed = ControllerSpeed;
}