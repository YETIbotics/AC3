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


   
  private:
  	Robot robot;
    
};

#endif