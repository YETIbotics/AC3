#include "a_Robot.h"
#include "a_Controller.h"
#include "a_Lift.h"
#include "a_Drive.h"
#include "a_Intake.h"
#include "a_Claw.h"

Robot Robot;
Controller Controller(&Robot.Usb);
Lift Lift;
Drive Drive;
Intake Intake;
Claw Claw;

void setup() {
	Serial.begin(115200);
	while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
	
  Robot.SetController(&Controller);

}

void loop() {

	Robot.Read();

	Controller.Task();
  Lift.Task();
  Drive.Task();
  Intake.Task();
  Claw.Task();

  Robot.Write();

	Serial.println(Controller.LeftJoystick);
	delay(100);

}