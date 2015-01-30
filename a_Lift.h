#ifndef Lift_h
#define Lift_h

#include <Arduino.h>
#include "a_Robot.h"
#include "PID_v1.h"

class Lift 
{
  public:  
    // CONSTRUCTORS
    Lift(Robot *p); // Default pin selection.

    void Task();

    float ControllerSpeed;
   
  private:
  	Robot *robot;

  	//PID
    double liftCurPos = 0;
    double liftSetPoint = 0;
    double liftPIDOut = 0;
    bool liftPIDEnable = false;
    const int liftPIDTolerence = 5;
    const double liftKP = 1;
    const double liftKI = 1;
    const double liftKD = 1;

    PID liftPID;
    
};

#endif