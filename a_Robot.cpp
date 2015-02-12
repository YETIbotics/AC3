

#include "a_Robot.h"

// Constructors ////////////////////////////////////////////////////////////////

Robot::Robot()

{
	Usb.Init();

    mc1.init(_mc1_INA1, _mc1_INB1, _mc1_EN1DIAG1, _mc1_CS1, _mc1_INA2, _mc1_INB2, _mc1_EN2DIAG2, _mc1_CS2, _mc1_PWM1, _mc1_PWM2);
    mc2.init(_mc2_INA1, _mc2_INB1, _mc2_EN1DIAG1, _mc2_CS1, _mc2_INA2, _mc2_INB2, _mc2_EN2DIAG2, _mc2_CS2, _mc2_PWM1, _mc2_PWM2);

    encDriveRight.init(_drive_Right_encInt, _drive_Right_encDig);
    encDriveLeft.init(_drive_Left_encInt, _drive_Left_encDig);
    encLiftRight.init(_lift_Right_encInt, _lift_Right_encDig);
    encLiftLeft.init(_lift_Left_encInt, _lift_Left_encDig);
    encClaw.init(_claw_encInt, _claw_encDig);

    prevDriveRightSpeed = 0;
    prevDriveLeftSpeed = 0;
    prevLiftLeftSpeed = 0;
    prevLiftRightSpeed = 0;

    DriveRightSpeed = 0;
    DriveLeftSpeed = 0;
    LiftLeftSpeed = 0;
    LiftRightSpeed = 0;
    IntakeSpeed = 0;
    ArmSpeed = 0;
    ClawPower = 0;

    //Expect 0-255
    LEDRed = 0;
    LEDBlue = 0;
    LEDGreen = 0;

    TorqueLimitDrive = 0;
    TorqueLimitLift = 0;

}

void Robot::init(){

    serIntake.attach(_intake_PWM);
    serIntake.write(90);

    serArm.attach(_arm_PWM);
    serArm.write(90);

    serClaw.attach(_claw_PWM);
    serClaw.write(90);

    pinMode(_ledGrn, OUTPUT);
    pinMode(_ledBlu, OUTPUT);
    pinMode(_ledRed, OUTPUT);

    analogWrite(_ledRed, LEDRed);
    analogWrite(_ledGrn, LEDGreen);
    analogWrite(_ledBlu, LEDBlue);
}

void Robot::Read(){
	Usb.Task();

	_encDriveRight = encDriveRight.read() * -1;
    _encDriveLeft = encDriveLeft.read();
    _encLiftRight = encLiftRight.read();
    _encLiftLeft = encLiftLeft.read() * -1;
    _encClaw = encClaw.read();
    _leftLimitSwitch = digitalRead(_lift_Left_Limit);
    _rightLimitSwitch = digitalRead(_lift_Right_Limit);
    _driveLeftCurrent = mc2.getM2CurrentMilliamps();
    _driveRightCurrent = mc1.getM1CurrentMilliamps();
    _liftLeftCurrent = mc2.getM1CurrentMilliamps();
    _liftRightCurrent = mc1.getM2CurrentMilliamps();

/*
    Serial.print(_encDriveLeft);
    Serial.print("\t");

    Serial.print(_encDriveRight);
    Serial.print("\t");

    Serial.print(_encLiftLeft);
    Serial.print("\t");

    Serial.print(_encLiftRight);
    Serial.print("\t");

    Serial.println(_encClaw);
*/

}
void Robot::SetLED(int red, int grn, int blu)
{
    LEDRed = red;
    LEDBlue = blu;
    LEDGreen = grn;
}

float Robot::torqueLimit(float prevVal, float curVal, int torqueLim)
{
    float retVal = 0;

    //if torqueLim > 0, then it's enabled
    if(torqueLim > 0)
    {
        //If change is from negative to positive, or positive to negative
        //then pretend we were previously at zero. 
        if((curVal > 0 && prevVal < 0) || (curVal < 0 && prevVal > 0))
            prevVal = 0;


        //if is increase in power that is greater than torque limit
        if(prevVal >= 0 && curVal > 0 && curVal > prevVal && curVal > (prevVal+torqueLim))
        {
            //increase forward
            retVal = prevVal + torqueLim;
        }
        else if(prevVal <= 0 && curVal < 0 && curVal < prevVal && curVal < (prevVal-torqueLim))
        {
            //increase in reverse
            retVal = prevVal - torqueLim;
        }
        else //decrease in power
        {
            retVal = curVal;
        }
    }
    else
    {
        retVal = curVal;
    }

    return retVal;
}

void Robot::Write(){
	
	//DriveRightSpeed
	if(DriveRightSpeed < -400)
		DriveRightSpeed = -400;
    if(DriveRightSpeed > 400)
      DriveRightSpeed = 400;

  prevDriveRightSpeed = torqueLimit(prevDriveRightSpeed, DriveRightSpeed, TorqueLimitDrive);

  mc1.setM1Speed(prevDriveRightSpeed);

    //Serial.print(DriveRightSpeed);
    //Serial.print("\t");

    //DriveLeftSpeed
  if(DriveLeftSpeed < -400)
    DriveLeftSpeed = -400;
if(DriveLeftSpeed > 400)
    DriveLeftSpeed = 400;

prevDriveLeftSpeed = torqueLimit(prevDriveLeftSpeed, DriveLeftSpeed, TorqueLimitDrive);

mc2.setM2Speed(prevDriveLeftSpeed);

    //Serial.print(DriveLeftSpeed);
    //Serial.print("\t");

    //LiftLeftSpeed
if(LiftLeftSpeed < -400)
    LiftLeftSpeed = -400;
if(LiftLeftSpeed > 400)
    LiftLeftSpeed = 400;

prevLiftLeftSpeed = torqueLimit(prevLiftLeftSpeed, LiftLeftSpeed, TorqueLimitLift);

mc2.setM1Speed(prevLiftLeftSpeed);

    //Serial.print(LiftLeftSpeed);
    //Serial.print("\t");

    //LiftRightSpeed
if(LiftRightSpeed < -400)
    LiftRightSpeed = -400;
if(LiftRightSpeed > 400)
    LiftRightSpeed = 400;

prevLiftRightSpeed = torqueLimit(prevLiftRightSpeed, LiftRightSpeed, TorqueLimitLift);

mc1.setM2Speed(prevLiftRightSpeed);

    //Serial.print(LiftRightSpeed);
    //Serial.print("\t");

    //IntakeSpeed
if(IntakeSpeed < -400)
    IntakeSpeed = -400;
if(IntakeSpeed > 400)
    IntakeSpeed = 400;
serIntake.write(convertToServo(IntakeSpeed));



    //ArmSpeed
if(ArmSpeed < -400)
    ArmSpeed = -400;
if(ArmSpeed > 400)
    ArmSpeed = 400;
serArm.write(convertToServo(ArmSpeed));

    //Serial.println(ArmSpeed);
    //Serial.print("\t");

    //ClawPower
if(ClawPower < -400)
    ClawPower = -400;
if(ClawPower > 400)
    ClawPower = 400;
serClaw.write(convertToServo(ClawPower));

    //Serial.println(ClawPower);


analogWrite(_ledRed, LEDRed);
analogWrite(_ledGrn, LEDGreen);
analogWrite(_ledBlu, LEDBlue);


    //Expect 0-255
    /*float LEDRed;
    float LEDBlue;
    float LEDGreen;*/
    
}

int Robot::convertToServo(float inVal)
{

    if(inVal > 0) 
    {
        //Serial.println(((inVal/400 * (_servoMax - _servoNeut)) + _servoNeut));
        return ((inVal/400 * (_servoMax - _servoNeut)) + _servoNeut);
    }
    else if(inVal < 0)
    {
        //Serial.println(((inVal/400 * (_servoNeut - _servoMin)) + _servoNeut));
        return ((inVal/400 * (_servoNeut - _servoMin)) + _servoNeut);
    }
    else
        return _servoNeut;
}


//ReadOnly Methods
float Robot::GetEncDriveRight(){
	return _encDriveRight;
}

float Robot::GetEncDriveLeft(){
	return _encDriveLeft;
}

float Robot::GetEncLiftRight(){
	return _encLiftRight;
}

float Robot::GetEncLiftLeft(){
	return _encLiftLeft;
}

float Robot::GetEncClaw(){
	return _encClaw;
}

bool Robot::GetLeftLimitSwitch(){
	return _leftLimitSwitch;
}

bool Robot::GetRightLimitSwitch(){
	return _rightLimitSwitch;
}

float Robot::GetDriveLeftCurrent(){
	return _driveLeftCurrent;
}

float Robot::GetDriveRightCurrent(){
	return _driveRightCurrent;
}

float Robot::GetLiftLeftCurrent(){
	return _liftLeftCurrent;
}

float Robot::GetLiftRightCurrent(){
	return _liftRightCurrent;
}


/*
void Robot::SetController(Controller *p)
{
	controller = p;
}
*/