/*
  This Sketch written by the CREATE foundation iss based upon the work of an example sketch for the PS3 Bluetooth library - developed by Kristian Lauszus
 Most all of the original code was kept for reference.  If un-needed for this application it was commented out.
 For more information about Kristian Lauszus and this sketch visit my blog: http://blog.tkjelectronics.dk/ or 
 send him an e-mail:  kristianl@tkjelectronics.com
 For more about the CREATE Foundation and learn about it's technology education mission, visit: www.CREATE-Found.org.
 
 FEATURES
    ** Motor port 1-8 are mapped to controller
               1 - Right Y axis joystick
               2-  Left Y axis joystick
               3 - Triangle and Cross
               4 - Up and Down
               5 - L1 and L2
               6 - R1 and R2
               7 - Circle and Square
               8 - Left and Right
    ** Sensor ports 1-6 are mapped to allow a jumper to reverse motor direction on the corresponding motor ports
    ** Controller battery level displayed via the 4 LEDs on the remote
               1 - Dying
               2 - Low
               3 - Medium
               4 - High
    ** Example of how to turn rumble on and off.  Rumble high is turned on for half a second, then a 1/4 second pause and then rumble low is turned on for 1 second.
    ** Heartbeat to allow for faster relinking in the case of lost link    
    
 */

#include "PS3BT.h"
#include <Servo.h>
USB Usb;
BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
/* You can create the instance of the class in two ways */
//PS3BT PS3(&Btd); // This will just create the instance
PS3BT PS3(&Btd,0x00,0x1F,0x81,0x00,0x08,0x30); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch

Servo S1, S2, S3, S4, S5, S6, S7, S8;
int pos1 = 90, pos2 = 90, pos3 = 90, pos4 = 90, pos5 = 90, pos6 = 90, pos7 = 90, pos8 = 90;
int dir1 = 0, dir2 = 0, dir3 = 0, dir4 = 0, dir5 = 0, dir6 = 0;
boolean flag1 = true, flag4 = true;
boolean Pass = true;

int connectTime = 0;
byte battery=0, last_check=0;

// This is the millisecond count before the Heartbeat stops
int messageTimeout = 500;

void setup() 
{
Serial.begin(115200);
  if (Usb.Init() == -1) 
  {
    while(1); //halt
  }

  // Set the ports of the motors
  S1.attach(24);
  S2.attach(26);
  S3.attach(28);
  S4.attach(30);
  S5.attach(32);
  S6.attach(34);
  S7.attach(36);
  S8.attach(38);

  // Check the jumpers to determine direction of motors
  if (analogRead(A0) < 20) {dir1 = 0;}
  else {dir1 = 1;}
  delay(0130);
  
  if (analogRead(A1) < 20) {dir2 = 0;}
  else {dir2 = 1;}
  delay(0130);
  
  if (analogRead(A2) < 20) {dir3 = 0;}
  else {dir3 = 1;}
  delay(0130);
  
  if (analogRead(A3) < 20) {dir4 = 0;}
  else {dir4 = 1;}
  delay(0130);
  
  if (analogRead(A4) < 20) {dir5 = 0;}
  else {dir5 = 1;}
  delay(0130);
  
  if (analogRead(A5) < 20) {dir6 = 0;}
  else {dir6 = 1;}
}

void loop() 
{
  Usb.Task();

  if(PS3.PS3Connected) 
  {
     // If the last message was received more than messageTimeout, 
     if(millis()-PS3.getLastMessageTime>messageTimeout && !Pass)
     {
        // Reset the flags that are used
        flag1 = true;
        flag4 = true;
        
        // Stop all motors from turning
        S1.write(90);
        S2.write(90);
        S3.write(90);
        S4.write(90);
        S5.write(90);
        S6.write(90);
        S7.write(90);
        S8.write(90);
        
        // Officially disconnect
        PS3.disconnect();
        
        // Reset check variable
        Pass = true;
     }
     //
     // Normal operating code goes in the 'else' statement below
     //
     else
     {
         
       // Upon linking, vibrate the controller
       if(flag1){
          connectTime = millis();        // Note the connection time
          flag1 = false;                 
          PS3.setRumbleOn(RumbleHigh);   // Set the rumble high
          delay(500);
          PS3.setRumbleOff();            // Turn off rumble
          delay(250);
          PS3.setRumbleOn(RumbleLow);    // Set the rumble low
          delay(1000);
          PS3.setRumbleOff();            // Turn off rumble
          }
  
        // Every 60 seconds update the battery indicator lights along the top  
        if(flag4 == true)
        {
          if(PS3.getStatus(Dying))    { battery = 1;}
          else if(PS3.getStatus(Low)) { battery = 2;}
          else if(PS3.getStatus(High)){ battery = 3;}
          else if(PS3.getStatus(Full)){ battery = 4;}
          PS3.setLedOn(LED1);
          if(battery == 2 || battery == 3 || battery == 4){PS3.setLedOn(LED2);}
          if(battery == 3 || battery == 4){PS3.setLedOn(LED3);}
          if(battery == 4){PS3.setLedOn(LED4);}
          flag4 = false;
        }
        
        if(millis()/60000 != last_check)
        {
          flag4 = true;
          last_check = millis()/60000;
        }
    
        // Motor 1    
        if (dir1 == 0)
        {
          pos1 = map(PS3.getAnalogHat(RightHatY), 0, 255, 45, 135);
        }
        else
        {
          pos1 = map(PS3.getAnalogHat(RightHatY), 0, 255, 135, 45);
        } 
        
        // Motor 2
        if (dir2 == 0)
        {
          pos2 = map(PS3.getAnalogHat(LeftHatY), 0, 255, 135, 45);
        }
        else 
        {
          pos2 = map(PS3.getAnalogHat(LeftHatY), 0, 255, 45, 135); 
        }
       
        // Motor 3
        pos3 = 90;
        if(PS3.getButtonPress(TRIANGLE))
        {
         if(dir3 == 0)
         {
           pos3 = 45;
         }
         else
         { 
           pos3 = 135;
         }
        }  
        if(PS3.getButtonPress(CROSS))
        {
          if (dir3 == 0)
          {
            pos3 = 135;
          }
          else
          {
            pos3 = 45;
          }
        }  
       
        // Motor 4
        pos4 = 90;
        if(PS3.getButtonPress(UP))
        {
          if(dir4 == 0)
          {
            pos4 = 45;
          }
         else
         {
           pos4 = 135;
         }
        }
        if(PS3.getButtonPress(DOWN))
        {
          if (dir4 == 0)
          { 
            pos4 = 135;
          }
          else
          {
            pos4 = 45;
          }
        } 
       
        // Motor 5
        pos5 = 90;
        if (dir5 == 0)
        {
          pos5 = map(PS3.getAnalogButton(L2_ANALOG), 0, 255, 90, 135);
        }
        else
        { 
          pos5 = map(PS3.getAnalogButton(L2_ANALOG), 0, 255, 90, 45); 
        }
         
        if(PS3.getButtonPress(L1))
        {
           if (dir5 == 0)
           {
             pos5 = 45;
           }
           else
           {
             pos5 = 135;
           }
        }          
       
        // Motor 6
        pos6 = 90;
        if (dir6 == 0)
        {
          pos6 = map(PS3.getAnalogButton(R2_ANALOG), 0, 255, 90, 135);
        }
        else
        {
          pos6 = map(PS3.getAnalogButton(R2_ANALOG), 0, 255, 90, 45); 
        }
  
        if(PS3.getButtonPress(R1)) 
        {
          if (dir6 == 0) 
          {
            pos6 = 45;
          }
          else
          {
            pos6 = 135; 
          }
        }   
    
        // Motor 7
        pos7 = 90;
        if(PS3.getButtonPress(CIRCLE)) {pos7 = 45;}
     
        if(PS3.getButtonPress(SQUARE)) {pos7 = 135;}  
            
        // Motor 8
        pos8 = 90;
        if(PS3.getButtonPress(LEFT)) {pos8 = 45;}
        
        if(PS3.getButtonPress(RIGHT)) {pos8 = 135;}  
    
        // Set the deadbands on the controller
        if (pos1 < 100 && pos1 > 80) {pos1 = 90;}
        if (pos2 < 100 && pos2 > 80) {pos2 = 90;}  
        if (pos5 < 95 && pos5 > 85) {pos5 = 90;}  
        if (pos6 < 95 && pos6 > 85) {pos6 = 90;}  
    
        // Write the motor positions
        S1.write(pos1);
        S2.write(pos2);
        S3.write(pos3);
        S4.write(pos4);
        S5.write(pos5);
        S6.write(pos6);
        S7.write(pos7);
        S8.write(pos8);
    
        // If it is requested, disconnect
        if(PS3.getButtonClick(PS)) 
        {
          // Reset the flags that are used
          flag1 = true;
          flag4 = true;
          
          // Stop all motors from turning
          S1.write(90);
          S2.write(90);
          S3.write(90);
          S4.write(90);
          S5.write(90);
          S6.write(90);
          S7.write(90);
          S8.write(90);
          
          // Officially disconnect
          PS3.disconnect();
          
          // This resets the heartbeat message
          Pass = true;
        } 
     }
  }
}


