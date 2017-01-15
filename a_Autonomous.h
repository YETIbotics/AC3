#ifndef Autonomous_h
#define Autonomous_h

#include <Arduino.h>
#include "a_Robot.h"

class Autonomous 
{
  public:  
    // CONSTRUCTORS
    Autonomous(Robot *p); // Default pin selection.

    void Task();

   
  private:
  	Robot *robot;
    
};

#endif