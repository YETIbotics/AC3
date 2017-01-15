

//Indirectly Used Library Includes
#include "XBOXRECV.h"
#include "DualVNH5019MotorShield.h"
#include "SimpleTimer.h"
#include "PID_v1.h"
#include "Encoder.h"

//Directly Used Library Includes
#include "a_Robot.h"
#include "a_Controller.h"

#include "a_Lift.h"
#include "a_Drive.h"
#include "a_Intake.h"
#include "a_Claw.h"
#include <Servo.h>


Robot Robot;
Controller Controller(&Robot.Usb);

Lift Lift(&Robot);
Drive Drive(&Robot);
Intake Intake(&Robot);
Claw Claw(&Robot);

SimpleTimer timer;

void setup() {
	Serial.begin(115200);
	while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection


  Robot.init();
  Claw.init();
  Lift.init();
  Drive.init();

  Robot.SetLED(255,255,255); //Set LEDs to white

  timer.setInterval(20, runStuff);
  timer.setInterval(1000, manageLEDs);
  timer.setInterval(250, blink);
  
}

int secCount = 2; //default to 2, takes ~ 1.5 secs to boot
bool blinkLEDs = false;
int red, blu, grn;
bool blnkOn = true;
void manageLEDs()
{
  secCount++;



  //25 second auton
  //80 Second Drivers
  switch(secCount)
  {
    case 1:
      //LEDs green
    grn = 255;
    red = 0;
    blu = 0;
    Robot.SetLED(red,grn,blu);
    break;
    case 20:
      //Blink Green
    blinkLEDs = true;
    break;
    case 25:
      //LEDs White
    blinkLEDs = false;
    red = 255;
    blu = 255;
    grn = 255;
    Robot.SetLED(red,grn,blu);
    break;
    case 75:
      //Blink LEDs
    blinkLEDs = true;
    break;
    case 105:
      //LEDs White
    blinkLEDs = false;
    red = 255;
    blu = 255;
    grn = 255;
    Robot.SetLED(red,grn,blu);
    break;
  }
}
void blink()
{

  if(blinkLEDs)
  {
    if(blnkOn)
    {
      blnkOn = false;
      Robot.SetLED(red*.1,grn*.1,blu*.1);
    }
    else
    {
      blnkOn = true;
      Robot.SetLED(red,grn,blu);
    }
  }
}

void runStuff()
{
  Robot.Read();
  Controller.Task();

  MapRobot();

  Lift.Task();
  Drive.Task();
  Intake.Task();
  Claw.Task();

  Robot.Write();

  SerialReceiveMove();

}

void loop() {

  timer.run();

}

void MapRobot()
{
  Drive.LeftControllerSpeed = Controller.LeftJoystick;
  Drive.RightControllerSpeed = Controller.RightJoystick;

  Lift.ControllerSpeed = Controller.TriggerAggregate;

  Intake.ControllerSpeed = Controller.LR2Aggregate;

  Claw.ControllerClawPosition = Controller.APress;
  Claw.ControllerArmSpeed = Controller.DPadLeftRight;

  Claw.ControllerYPress = Controller.YPress;

}

/********************************************
 * Serial Communication functions / helpers
 ********************************************/


union {                // This Data structure lets
  byte asBytes[24];    // us take the byte array
  float asFloat[6];    // sent from processing and
}                      // easily convert it to a
foo;                   // float array

void SerialReceiveMove()
{

///////////////////////////////////

  String readString, funcName;
  int funcVal;

  readString="";

  while (Serial.available()) {
    delay(10);  //delay to allow buffer to fill 
    if (Serial.available() >0) {
      char c = Serial.read();  //gets one byte from serial buffer
      readString += c; //makes the string readString
    } 
  }

  if (readString.length() >0) {
      Serial.println(readString); //see what was received
      
      // expect a string like XX #### containing the two servo positions      
      funcName = readString.substring(0, 2); //get the first four characters
      String tmpFuncVal = readString.substring(3, 7); //get the next four characters 
      funcVal = tmpFuncVal.toFloat();


    } 
    Serial.flush();  

    if (funcName == "LT")
    {
      Lift.LiftTo(funcVal);
    }
    else if (funcName == "LA")
    {
      Lift.LiftAdd(funcVal);
    }
    else if (funcName == "DL")
    {
      Drive.DriveLeft(funcVal);
    }
    else if (funcName == "DR")
    {
      Drive.DriveRight(funcVal);
    }
    else if (funcName == "MO")
    {
      Drive.Move(funcVal);
    }
    else if (funcName == "CL")
    {
      Claw.Clamp();
    }
    else if (funcName == "DC")
    {
      Claw.DeClamp();
    }
    else if (funcName == "AT")
    {
      Claw.ArmTo(funcVal);
    }
    else if (funcName == "SC")
    {
      Claw.StopClamp();
    }
  else if (funcName == "TL") //Torque Limit Lift
  {
    Lift.SetTorqueLimit(funcVal);
  }
  else if (funcName == "TD") //Torque Limit Drive
  {
    Drive.SetTorqueLimit(funcVal);
  }
  else if (funcName == "AR")
  {
    void autoRedGoal();
  }


}
