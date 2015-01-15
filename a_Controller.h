#ifndef Controller_h
#define Controller_h

#include <Arduino.h>
#include "XBOXRECV.h"


class Controller
{
  public:  
    // CONSTRUCTORS
    Controller(USB *pUsb); // Default pin selection.

    void Task();


    float LeftJoystick;
    float RightJoystick;
    float TriggerAggregate;
    int LR2Aggregate;

   private:

    XBOXRECV Xbox;
    const int joystickMinThresh = 1500;

};

#endif