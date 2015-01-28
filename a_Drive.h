#ifndef Drive_h
#define Drive_h

#include <Arduino.h>
#include "a_Robot.h"

class Drive 
{
  public:  
    // CONSTRUCTORS
    Drive(Robot *p); // Default pin selection.

    void Task();

    float LeftControllerSpeed;
    float RightControllerSpeed;
   
  private:
  	Robot *robot;
    
};

#endif