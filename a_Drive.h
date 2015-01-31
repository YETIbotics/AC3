#ifndef Drive_h
#define Drive_h

#include <Arduino.h>
#include "a_Robot.h"
#include "PID_v1.h"

class Drive 
{
  public:  
    // CONSTRUCTORS
    Drive(Robot *p); // Default pin selection.

    void Task();
    void init();
    void GoTo(double position);

     PID drivePID; 

    double driveCurPos = 0;
    double driveSetPoint = 0;
    double drivePIDOut = 0;

    float LeftControllerSpeed;
    float RightControllerSpeed;
   
  private:
  	Robot *robot;

  	//Drive PID
  
    bool drivePIDEnable = false;
    const int drivePIDTolerence = 5;
    const double driveKP = 1;
    const double driveKI = 1;
    const double driveKD = 1;

    
};

#endif