#include "a_Robot.h"

// Constructors ////////////////////////////////////////////////////////////////

Robot::Robot()
{
	
	Serial.begin(115200);
	while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
	if (Usb.Init() == -1) {
		Serial.print(F("\r\nOSC did not start"));
		while (1); //halt
	}
}