#ifndef Claw_h
#define Claw_h

#include <Arduino.h>
#include "a_Robot.h"

class Claw 
{
  public:  
    // CONSTRUCTORS
    Claw(Robot *p); // Default pin selection.

    void Task();
   
  private:
  	Robot *robot;
    
};

#endif