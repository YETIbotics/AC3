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
    void init();

    //Autonomous Methods
    void LiftTo(double position);
    void LiftAdd(double position);

    //PID

     PID liftPID; 

    double liftCurPos = 0;
    double liftSetPoint = 0;
    double liftPIDOut = 0;

    float ControllerSpeed;


   
  private:
  	Robot *robot;

  	//Lift PID
  
    bool liftPIDEnable = false;
    const int liftPIDTolerence = 0;
    const double liftKP = 60;
    const double liftKI = 0;
    const double liftKD = 1;



   


};





#endif