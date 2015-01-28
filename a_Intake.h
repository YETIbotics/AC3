#ifndef Intake_h
#define Intake_h

#include <Arduino.h>
#include "a_Robot.h"

class Intake 
{
  public:  
    // CONSTRUCTORS
    Intake(Robot *p); // Default pin selection.

    void Task();

    int ControllerSpeed;
   
  private:
  	Robot *robot;
    
};

#endif