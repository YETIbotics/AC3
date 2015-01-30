#ifndef Claw_h
#define Claw_h

#include <Arduino.h>
#include "a_Robot.h"
#include "SimpleTimer.h"
#include "PID_v1.h"

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

    //PID
    double armCurPos = 0;
    double armSetPoint = 0;
    double armPIDOut = 0;
    bool armPIDEnable = false;
    const int armPIDTolerence = 5;
    const double armKP = 1;
    const double armKI = 1;
    const double armKD = 1;

    PID armPID;

  	void turnOffClaw();


    
};

#endif