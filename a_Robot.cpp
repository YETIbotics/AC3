#include "a_Robot.h"

// Constructors ////////////////////////////////////////////////////////////////

Robot::Robot()
: mc1(_mc1_INA1, _mc1_INB1, _mc1_EN1DIAG1, _mc1_CS1, _mc1_INA2, _mc1_INB2, _mc1_EN2DIAG2, _mc1_CS2, _mc1_PWM1, _mc1_PWM2)
, mc2(_mc2_INA1, _mc2_INB1, _mc2_EN1DIAG1, _mc2_CS1, _mc2_INA2, _mc2_INB2, _mc2_EN2DIAG2, _mc2_CS2, _mc2_PWM1, _mc2_PWM2)
, encDriveRight(_drive_Right_encInt, _drive_Right_encDig)
, encDriveLeft(_drive_Left_encInt, _drive_Left_encDig)
, encLiftRight(_lift_Right_encInt, _lift_Right_encDig)
, encLiftLeft(_lift_Left_encInt, _lift_Left_encDig)
{
	Usb.Init();
}

void Robot::Read(){
	Usb.Task();
}

void Robot::Write(){

	//float asdf = controller->LeftJoystick;
}

void Robot::SetController(Controller *p)
{
	controller = p;
}