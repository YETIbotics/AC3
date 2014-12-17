

#include "XBOXRECV.h"
#include "DualVNH5019MotorShield.h"
#include "SimpleTimer.h"
#include "PID_v1.h"
#include "Encoder.h"
// Satisfy IDE, which only needs to see the include statment in the ino.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif


// Motor Controller 1 Pinouts 
const int _mc1_INA1 = 48;
const int _mc1_INB1 = 24;
const int _mc1_EN1DIAG1 = 26;
const int _mc1_CS1 = A4;
const int _mc1_INA2 = 22;
const int _mc1_INB2 = 52;
const int _mc1_EN1DIAG2 = 40;
const int _mc1_CS2 = A2; 
const int _mc1_PWM1 = 10;
const int _mc1_PWM2 = 12;

// Motor Controller 2 Pinouts
const int _mc2_INA1 = 42;
const int _mc2_INB1 = 30;
const int _mc2_EN1DIAG1 = 32;
const int _mc2_CS1 = A5;
const int _mc2_INA2 = 28;
const int _mc2_INB2 = 46;
const int _mc2_EN1DIAG2 = 44;
const int _mc2_CS2 = A3; 
const int _mc2_PWM1 = 9;
const int _mc2_PWM2 = 11;

// Encoder Pinouts
const int _encRDInt = 3;
const int _encRDDig = 22;

const int _encRLInt = 2;
const int _encRLDig = 24;

const int _encLDInt = 18;
const int _encLDig = 29;

const int _encLLInt = 30;
const int _encLLDig = 18;

// Limit Switch Pins
const int _leftLimitSwitch = 0;
const int _rightLimitSwitch = 0;

// Max Lift 
const int _liftMax = 50;

// Timer Polling Intervals
const int _readControllerInterval = 10;
const int _writeControllerInterval = 15;
const int _writeRobotInterval = 20;
const int _readRobotInterval = 25;



float _leftSpeed;
float _rightSpeed;
float _liftSpeed;

float _leftLiftPos;
float _rightLiftPos;



USB Usb;
XBOXRECV Xbox(&Usb);

DualVNH5019MotorShield md1(_mc1_INA1, _mc1_INB1, _mc1_EN1DIAG1
 , _mc1_CS1, _mc1_INA2, _mc1_INB2, _mc1_EN1DIAG2, _mc1_CS2, _mc1_PWM1, _mc1_PWM2);

DualVNH5019MotorShield md2(_mc2_INA1, _mc2_INB1, _mc2_EN1DIAG1
 , _mc2_CS1, _mc2_INA2, _mc2_INB2, _mc2_EN1DIAG2, _mc2_CS2, _mc2_PWM1, _mc2_PWM2);

Encoder encRD(_encRDInt, _encRDDig);
Encoder encLD(_encLDInt, _encLDig);
Encoder encRL(_encRLInt, _encRLDig);
Encoder encLL(_encLLInt, _encLLDig);

SimpleTimer timer;



void setup() {


  // assign public state variables
  _leftSpeed = 0.0;
  _rightSpeed = 0.0;
  _liftSpeed = 0.0;
  _leftLiftPos = 0.0;
  _rightLiftPos = 0.0;   


  Serial.begin(115200);
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }

  md1.init();
  md2.init();

  timer.setInterval(_readControllerInterval, readController);
  timer.setInterval(_writeControllerInterval, writeController);
  timer.setInterval(_readRobotInterval, readRobot);
  timer.setInterval(_writeRobotInterval, writeRobot);

}



void loop() {

	timer.run();

}



void readController(){
  Usb.Task();

  if (Xbox.XboxReceiverConnected) 
  {
    for (uint8_t i = 0; i < 4; i++) 
    {
      if (Xbox.Xbox360Connected[i]) 
      {
        //Left Hat Y Axis
        if (Xbox.getAnalogHat(LeftHatY, i) > 7500 || Xbox.getAnalogHat(LeftHatY, i) < -7500) {
          _leftSpeed = 400.0 / 32767 * Xbox.getAnalogHat(LeftHatY, i); 
          
        }
        else
        {
          _leftSpeed = 0.0;
          
        }
        //Right Hat Y Axis
        if (Xbox.getAnalogHat(RightHatY, i) > 7500 || Xbox.getAnalogHat(RightHatY, i) < -7500) {
          _rightSpeed = 400.0 / 32767 * Xbox.getAnalogHat(RightHatY, i); 
        }
        else
        {
          _rightSpeed = 0.0;
        }
        //L2 Trigger
        if (Xbox.getButtonPress(L2, i)) 
        {
          _liftSpeed = 400.0 / 255 * Xbox.getButtonPress(L2, i) * -1; 
        }
        //R2 Trigger
        else if (Xbox.getButtonPress(R2, i)) 
        {
          _liftSpeed = 400.0 / 255 * Xbox.getButtonPress(R2, i); 
        }
        else
        {
          _liftSpeed = 0.0;
        }
      }
    }
  }
}

void writeController(){

}

void readRobot(){

}

void writeRobot(){
  MoveSpeed(_leftSpeed, _rightSpeed);
  LiftSpeed(_liftSpeed);
}




// ===========================================
// ROBOT METHODS
// ===========================================

 void MoveSpeed(float leftSpeed, float rightSpeed)
 {
  //Set Right Speed
  md2.setM2Speed(leftSpeed);

  //Set Left Speed
  md1.setM1Speed(rightSpeed);

 }

 void LiftSpeed(float liftSpeed)
 {
    //Set Lift Speed, Brake if Zero
    if(liftSpeed > -25 && liftSpeed < 25)
    {
      md2.setM1Brake(400);
      md1.setM2Brake(400);
    }
    else
    {
      md2.setM1Speed(liftSpeed);
      md1.setM2Speed(liftSpeed);
    }
 }
