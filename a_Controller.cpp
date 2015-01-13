#include "a_Controller.h"

// Constructors ////////////////////////////////////////////////////////////////

Controller::Controller(USB *p)
: Xbox(p)
{
	LeftJoystick = 0.0;
}

void Controller::Task()
{
	//This is where you update controller items.
	if (Xbox.XboxReceiverConnected) 
	{
		for (uint8_t i = 0; i < 4; i++) 
		{
			if (Xbox.Xbox360Connected[i]) 
			{
				if (Xbox.getAnalogHat(LeftHatY, i) > 1000 || Xbox.getAnalogHat(LeftHatY, i) < -1000) {
					LeftJoystick = 400.0 / 32767 * Xbox.getAnalogHat(LeftHatY, i); 
				}
				else
				{
					LeftJoystick = 0.0;
				}
			}
		}
	}
}