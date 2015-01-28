#ifndef Lift_h
#define Lift_h

#include <Arduino.h>
#include "a_Robot.h"

class Lift 
{
  public:  
    // CONSTRUCTORS
    Lift(Robot *p); // Default pin selection.

    void Task();

    float ControllerSpeed;
   
  private:
  	Robot *robot;
    
};

#endif