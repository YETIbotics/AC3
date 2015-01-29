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
    void init();

    int ControllerClawPosition;
    int ControllerArmSpeed;

    unsigned long delatch = 0;
   
  private:
  	Robot *robot;

  	int clawTimerId;


  	void turnOffClaw();
    
};

#endif