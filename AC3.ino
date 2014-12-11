/*
 Example sketch for the Xbox Wireless Reciver library - developed by Kristian Lauszus
 It supports up to four controllers wirelessly
 For more information see the blog post: http://blog.tkjelectronics.dk/2012/12/xbox-360-receiver-added-to-the-usb-host-library/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <XBOXRECV.h>
#include "DualVNH5019MotorShield.h"
// Satisfy IDE, which only needs to see the include statment in the ino.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif

// Motor Controller 1 Pinouts 
 const int _mc1_INA1 = 48;
 const int _mc1_INB1 = 24;
 const int _mc1_EN1DIAG1 = 26;
 const int _mc1_CS1 = A4;
 const int _mc1_INA2 = 22;
 const int _mc1_INB2 = 52;
 const int _mc1_EN1DIAG2 = 50;
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
 const int _mc2_EN1DIAG2 = 44;
 const int _mc2_CS2 = A3; 
 const int _mc2_PWM1 = 9;
 const int _mc2_PWM2 = 11;

float leftSpeed;
float rightSpeed;

 DualVNH5019MotorShield md1(_mc1_INA1, _mc1_INB1, _mc1_EN1DIAG1
   , _mc1_CS1, _mc1_INA2, _mc1_INB2, _mc1_EN1DIAG2, _mc1_CS2, _mc1_PWM1, _mc1_PWM2);
 DualVNH5019MotorShield md2(_mc2_INA1, _mc2_INB1, _mc2_EN1DIAG1
   , _mc2_CS1, _mc2_INA2, _mc2_INB2, _mc2_EN1DIAG2, _mc2_CS2, _mc2_PWM1, _mc2_PWM2);

 USB Usb;
 XBOXRECV Xbox(&Usb);




 void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nXbox Wireless Receiver Library Started"));

  md1.init();
  md2.init();

  leftSpeed = 0;
  rightSpeed = 0;

}


void loop() {

	
  Usb.Task();
  if (Xbox.XboxReceiverConnected) {
    for (uint8_t i = 0; i < 4; i++) {
      if (Xbox.Xbox360Connected[i]) {

       



        if (Xbox.getAnalogHat(LeftHatY, i) > 7500 || Xbox.getAnalogHat(LeftHatY, i) < -7500) {
         leftSpeed = map(Xbox.getAnalogHat(LeftHatY, i), 0, 32000, 0, 400);
          	//leftSpeed = 200;
            //Serial.print(F("LeftHatY: "));
            //Serial.print(leftSpeed);
            //Serial.print("\t");

       } 
       else {
          	// left motor 0 
        leftSpeed = 0;
       }

       if (Xbox.getAnalogHat(RightHatY, i) > 7500 || Xbox.getAnalogHat(RightHatY, i) < -7500) {
         rightSpeed = map(Xbox.getAnalogHat(RightHatY, i), 0, 32000, 0, 400);
          	//rightSpeed = 200;
           // Serial.print(F("RightHatY: "));
           // Serial.print(rightSpeed);

       }
       else {
          	// right motor 0
        rightSpeed = 0;
       }
          //Serial.println();



      //Right Drive
       md2.setM2Speed(leftSpeed);

       //Left Drive
       md1.setM1Speed(rightSpeed);

	     //Set Left Speed
       //md2.setM2Speed(rightSpeed);


       //md1.setM1Speed(leftSpeed);

       

       Serial.print(F("LeftSpeed: "));
       Serial.print(leftSpeed);
       Serial.print("\t");
       Serial.print(F("RightSpeed: "));
       Serial.print(rightSpeed);
       Serial.println();

       delay(10);
     }



   }
 }

}

