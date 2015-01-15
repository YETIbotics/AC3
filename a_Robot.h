#ifndef Robot_h
#define Robot_h

#include <Arduino.h>
#include "Usb.h"


class Robot 
{
  public:  
    // CONSTRUCTORS
    Robot(); // Default pin selection.

    void Task();

    USB Usb;
   
  private:
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

    // Encoder Pinouts
    const int _drive_Right_encInt = 3;
    const int _drive_Right_encDig = 23;

    const int _lift_Right_encInt = 2;
    const int _lift_Right_encDig = 25;

    const int _drive_Left_encInt = 18;
    const int _drive_Left_encDig = 27;

    const int _lift_Left_encInt = 19;
    const int _lift_Left_encDig = 29;

    // Limit Switch Pins
    const int _lift_Left_Limit = 0;
    const int _lift_Right_Limit = 0;

    
};

#endif