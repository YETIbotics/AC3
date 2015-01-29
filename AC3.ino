

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



void setup() {
	Serial.begin(115200);
	while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection


  Robot.init();
  Claw.init();
  //Robot.SetController(&Controller);
  //Servo sIntake;
  //sIntake.attach(7);
  //sIntake.write(90);
  //delay(500);
  //sIntake.write(29);

}

void loop() {

	Robot.Read();
	Controller.Task();

  MapRobot();

  Lift.Task();
  Drive.Task();
  Intake.Task();
  Claw.Task();

  Robot.Write();


	//Serial.print("Robot. - DriveLeftSpeed :: ");
  //Serial.println(Robot.DriveLeftSpeed);
  

	delay(20);

}

void MapRobot()
{
  Drive.LeftControllerSpeed = Controller.LeftJoystick;
  Drive.RightControllerSpeed = Controller.RightJoystick;

  Lift.ControllerSpeed = Controller.TriggerAggregate;

  Intake.ControllerSpeed = Controller.LR2Aggregate;

  Claw.ControllerClawPosition = Controller.APress;
  Claw.ControllerArmSpeed = Controller.DPadLeftRight;

}