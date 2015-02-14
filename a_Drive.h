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




    //Autonomous Methods
    void DriveLeft(double position);
    void DriveRight(double position);
    void Move(double position);

    void SetTorqueLimit(int val);


    //Floats
    float LeftControllerSpeed;
    float RightControllerSpeed;

    //DriveLeftPID

     PID driveLeftPID; 

    double driveLeftCurPos = 0;
    double driveLeftSetPoint = 0;
    double driveLeftPIDOut = 0;
    double driveLeftCorrSetPoint = 0;



    //DriveRightPID

    PID driveRightPID;
    double driveRightCurPos = 0;
    double driveRightSetPoint = 0;
    double driveRightPIDOut = 0;
    double driveRightCorrSetPoint = 0;
   
  private:
  	Robot *robot;

  	//DriveLeftPID
  
    bool driveLeftPIDEnable = false;
    const int driveLeftPIDTolerence = 0;
    const double driveLeftKP = 10;
    const double driveLeftKI = 1;
    const double driveLeftKD = 1;

    //DriveRightPID

    bool driveRightPIDEnable = false;
    const int driveRightPIDTolerence = 0;
    const double driveRightKP = 10;
    const double driveRightKI = 1;
    const double driveRightKD = 1;

    int correctionVal = 0;


    
};

#endif