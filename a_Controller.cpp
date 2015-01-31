#include "a_Controller.h"

// Constructors ////////////////////////////////////////////////////////////////

Controller::Controller()
:Xbox(&dummyXboxPointer)
{

}

Controller::Controller(USB *p)
: Xbox(p)
{
	LeftJoystick = 0.0;
	RightJoystick = 0.0;
	TriggerAggregate = 0.0;
	LR2Aggregate = 0;
	DPadLeftRight = 0;
	APress = 0;
	YPress = 0;
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
				if (Xbox.getAnalogHat(LeftHatY, i) > joystickMinThresh || Xbox.getAnalogHat(LeftHatY, i) < -joystickMinThresh) {
					LeftJoystick = 400.0 / 32767 * Xbox.getAnalogHat(LeftHatY, i); 
				}
				else
				{
					LeftJoystick = 0.0;
				}

				if (Xbox.getAnalogHat(RightHatY, i) > joystickMinThresh || Xbox.getAnalogHat(RightHatY, i) < -joystickMinThresh) {
					RightJoystick = 400.0 / 32767 * Xbox.getAnalogHat(RightHatY, i); 
				}
				else
				{
					RightJoystick = 0.0;
				}

				//L2 Trigger
		        if (Xbox.getButtonPress(R2, i)) 
		        {
		          TriggerAggregate = 400.0 / 255 * Xbox.getButtonPress(R2, i) * 1; 
		        }
		        //R2 Trigger
		        else if (Xbox.getButtonPress(L2, i)) 
		        {
		          TriggerAggregate = 400.0 / 255 * Xbox.getButtonPress(L2, i) * -1; 
		        }
		        else
		        {
		          TriggerAggregate = 0.0;
		        }

		        if (Xbox.getButtonPress(LEFT, i)) 
		        {
		          DPadLeftRight = -1; 
		        }
		        else if (Xbox.getButtonPress(RIGHT, i)) 
		        {
		          DPadLeftRight = 1; 
		        }
		        else
		        {
		          DPadLeftRight = 0;
		        }

		        if (Xbox.getButtonPress(L1, i))
		        {
		          LR2Aggregate = 1;
		        
		        }
		        //R1 Button
		        else if (Xbox.getButtonPress(R1, i))
		        {
		          LR2Aggregate = -1;
		          
		        }
		        else
		        {
		          LR2Aggregate = 0;
		          
		        }

		        if (Xbox.getButtonPress(A, i))
		        {
		          APress = 1;
		        }
		        else
		        {
		          APress = 0;
		        }

		        if (Xbox.getButtonPress(Y, i))
		        {
		          YPress = 1;
		        }
		        else
		        {
		          YPress = 0;
		        }


			}
		}
	}
}