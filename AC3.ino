#include "a_Robot.h"
#include "a_Controller.h"

#include "a_Lift.h"
#include "a_Drive.h"
#include "a_Intake.h"
#include "a_Claw.h"

//Indirectly Used Library Includes
#include <Servo.h>
#include "XBOXRECV.h"
#include "DualVNH5019MotorShield.h"
#include "Encoder.h"

Robot Robot;
Controller Controller(&Robot.Usb);

Lift Lift(&Robot);
Drive Drive(&Robot);
Intake Intake(&Robot);
Claw Claw(&Robot);

void setup() {
	Serial.begin(115200);
	while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection

  //Robot.SetController(&Controller);

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
  

	delay(100);

}

void MapRobot()
{
  Drive.LeftControllerSpeed = Controller.LeftJoystick;
  Drive.RightControllerSpeed = Controller.RightJoystick;

  Lift.ControllerSpeed = Controller.TriggerAggregate;

  Intake.ControllerSpeed = Controller.LR2Aggregate;


}