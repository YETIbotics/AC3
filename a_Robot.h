#ifndef Robot_h
#define Robot_h

#include <Arduino.h>
#include "Usb.h"
#include "DualVNH5019MotorShield.h"
#include "Encoder.h"
#include <Servo.h>
//#include "a_Controller.h"

class Robot 
{
  public:  
    // CONSTRUCTORS
    Robot(); // Default pin selection.


    void Read();
    void Write();
    void init();
     void SetLED(int red, int grn, int blu);

    //To be called by timer in INO to manage LED lights.
     void ManageLEDs();
     void BlinkLEDs();
    //void SetController(Controller *controller);

    //Read Only Items
    float GetEncDriveRight();
    float GetEncDriveLeft();
    float GetEncLiftRight();
    float GetEncLiftLeft();
    float GetEncClaw();
    bool GetLeftLimitSwitch();
    bool GetRightLimitSwitch();
    float GetDriveLeftCurrent();
    float GetDriveRightCurrent();
    float GetLiftLeftCurrent();
    float GetLiftRightCurrent();

    //Readable/Writeable Items

    //Expect -400 through 400
    float DriveRightSpeed;
    float DriveLeftSpeed;
    float LiftLeftSpeed;
    float LiftRightSpeed;
    float IntakeSpeed;
    float ArmSpeed;
    float ClawPower;

    //Expect 0-255
     int LEDRed;
     int LEDBlue;
     int LEDGreen;

    int TorqueLimitDrive;
    int TorqueLimitLift;

    USB Usb;
   
  private:
    //Controller *controller;
    DualVNH5019MotorShield mc1;
    DualVNH5019MotorShield mc2;
    Servo serIntake;
    Servo serArm;
    Servo serClaw;
    Encoder encDriveRight;
    Encoder encDriveLeft;
    Encoder encLiftRight;
    Encoder encLiftLeft;
    Encoder encClaw;

    float prevDriveRightSpeed;
    float prevDriveLeftSpeed;
    float prevLiftLeftSpeed;
    float prevLiftRightSpeed;

     int secCount;
     bool blinkLEDs;
     int red, blu, grn;
     bool blnkOn;

    //Private ReadOnly Vars
    int _encDriveRight;
    int _encDriveLeft;
    int _encLiftRight;
    int _encLiftLeft;
    int _encClaw;
    bool _leftLimitSwitch;
    bool _rightLimitSwitch;
    int _driveLeftCurrent;
    int _driveRightCurrent;
    int _liftLeftCurrent;
    int _liftRightCurrent;

    //Pirvate Methods
    int convertToServo(float inVal);
    float torqueLimit(float prevVal, float curVal, int torqueLim);

    //ESC
    const int _servoMax = 150;
    const int _servoMin = 29;
    const int _servoNeut = 90;


    // Motor Controller 1 Pinouts 
    const int _mc1_INA1 = 48;
    const int _mc1_INB1 = 24;
    const int _mc1_EN1DIAG1 = 26;
    const int _mc1_CS1 = A4;
    const int _mc1_INA2 = 22;
    const int _mc1_INB2 = 40;
    const int _mc1_EN2DIAG2 = 38;
    const int _mc1_CS2 = A2; 
    const int _mc1_PWM1 = 10;
    const int _mc1_PWM2 = 12;

    // Motor Controller 2 Pinouts
    const int _mc2_INA1 = 42;
    const int _mc2_INB1 = 30;
    const int _mc2_EN1DIAG1 = 32;
    const int _mc2_CS1 = A5;
    const int _mc2_INA2 = 28;
    const int _mc2_INB2 = 46;
    const int _mc2_EN2DIAG2 = 44;
    const int _mc2_CS2 = A3; 
    const int _mc2_PWM1 = 9;
    const int _mc2_PWM2 = 11;

    //Intake ESC Pin
    const int _intake_PWM = 7;
    const int _arm_PWM = 5;
    const int _claw_PWM = 6;

    // Encoder Pinouts
    const int _drive_Right_encInt = 3;
    const int _drive_Right_encDig = 23;

    const int _lift_Right_encInt = 2;
    const int _lift_Right_encDig = 25;

    const int _drive_Left_encInt = 18;
    const int _drive_Left_encDig = 27;

    const int _lift_Left_encInt = 19;
    const int _lift_Left_encDig = 29;

    const int _claw_encInt = 21;
    const int _claw_encDig = 49;

    // Limit Switch Pins
    const int _lift_Left_Limit = 37;
    const int _lift_Right_Limit = 35;

    //LED
    const int _ledRed = 4;
    const int _ledGrn = 8;
    const int _ledBlu = 13;

    
};

#endif