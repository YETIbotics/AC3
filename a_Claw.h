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
    void GoTo(double position);

    int ControllerClawPosition;
    int ControllerArmSpeed;
    int ControllerYPress;

     PID armPID;
     double armCurPos = 0;
    double armSetPoint = 0;
    double armPIDOut = 0;
   
  private:
  	Robot *robot;

  	int clawTimerId;

  	SimpleTimer timer;

    //PID
    
    bool armPIDEnable = false;
    const int armPIDTolerence = 5;
    const double armKP = 9;
    const double armKI = 1;
    const double armKD = 1;

   

  	void turnOffClaw();


    
};

#endif