#include "a_Robot.h"
#include "a_Controller.h"

Robot Robot;
Controller Controller(&Robot.Usb);

void setup() {
	Serial.begin(115200);
	while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
	
  Robot.SetController(&Controller);

}

void loop() {

	Robot.Read();
  
	Controller.Task();

  Robot.Write();

	Serial.println(Controller.LeftJoystick);
	delay(100);

}