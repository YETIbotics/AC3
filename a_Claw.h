#ifndef Claw_h
#define Claw_h

#include <Arduino.h>
#include "a_Robot.h"
#include "SimpleTimer.h"

class Claw 
{
  public:  
    // CONSTRUCTORS
    Claw(Robot *p); // Default pin selection.

    void Task();
    void init();

    int ControllerClawPosition;
    int ControllerArmSpeed;
   
  private:
  	Robot *robot;

  	int clawTimerId;

  	SimpleTimer timer;

  	void turnOffClaw();
    
};

#endif