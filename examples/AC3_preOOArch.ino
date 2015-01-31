
#include "XBOXRECV.h"
#include "DualVNH5019MotorShield.h"
#include "SimpleTimer.h"
#include "PID_v1.h"
#include "Encoder.h"
#include <Servo.h>
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
const int _mc1_INB2 = 40;
const int _mc1_EN2DIAG2 = 38;
const int _mc1_CS2 = A2; 
const int _mc1_PWM1 = 10;
const int _mc1_PWM2 = 12;
/*
 _INA1 = 2; 48
  _INB1 = 4; 24
  _EN1DIAG1 = 6; 26
  _CS1 = A0; A4
  _INA2 = 7; 22
  _INB2 = 8; 40
  _EN2DIAG2 = 12; 38
  _CS2 = A1; A2
  _PWM1 = 9; 10
  PWM2 = 10 ; 12
*/


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

const int _ledRed = 4;
const int _ledGrn = 8;
const int _ledBlu = 13;

//Intake ESC Pin
const int _intakePWM = 7;
const int _clawPWM = 5;
const int _clawSolenoidPWM = 6;

// Encoder Pinouts
const int _encRDInt = 3;
const int _encRDDig = 23;

const int _encRLInt = 2;
const int _encRLDig = 25;

const int _encLDInt = 18;
const int _encLDig = 27;

const int _encLLInt = 19;
const int _encLLDig = 29;

const int _encClawInt = 21;
const int _encClawDig = 49; //////////////////////

// Limit Switch Pins
const int _leftLimitSwitch = 37;
const int _rightLimitSwitch = 35;

// Max Lift 
const int _liftMax = 50;

//HobbyKing ESC Max & Min
//const float _ESC_HK_MAX = 180; //157.0;
//const float _ESC_HK_MIN = 0; //29.0;
const float _ESC_HK_ADD = 0;
const float _ESC_HK_MAX = 157.0+_ESC_HK_ADD;
const float _ESC_HK_MIN = 29.0-_ESC_HK_ADD;

// Timer Polling Intervals
const int _readControllerInterval = 10;
const int _writeControllerInterval = 15;
const int _writeRobotInterval = 20;
const int _readRobotInterval = 25;

//arm
const float _armMin = 47;
const float _armMax = 78;

bool _autoRunning = false;
int _autoProgNum = 0;
int _autoInterval = 0;
bool _pauseForPID = false;

const int _maxAutonomousDuration = 60000;



float _leftSpeed;
float _rightSpeed;
float _liftSpeed;

float _leftLiftPos;
float _rightLiftPos;

int _intakeDirection;

int _clawDirection;
int _clawSpeed;

int _redVal;
int _blueVal;
int _greenVal;

//Define Variables we'll be connecting to
double armSetpoint, armInput, armOutput;
double driveSetpoint, driveInput, driveOutput;
double liftSetpoint, liftInput, liftOutput;

//Specify the links and initial tuning parameters
PID armPID(&armInput, &armOutput, &armSetpoint,1,.75,.2, DIRECT);
PID drivePID(&driveInput, &driveOutput, &driveSetpoint,2,.05,.2, DIRECT);
PID liftPID(&liftInput, &liftOutput, &liftSetpoint,1,.75,.2, DIRECT);

USB Usb;
XBOXRECV Xbox(&Usb);

DualVNH5019MotorShield md1(_mc1_INA1, _mc1_INB1, _mc1_EN1DIAG1
 , _mc1_CS1, _mc1_INA2, _mc1_INB2, _mc1_EN2DIAG2, _mc1_CS2, _mc1_PWM1, _mc1_PWM2);

DualVNH5019MotorShield md2(_mc2_INA1, _mc2_INB1, _mc2_EN1DIAG1
 , _mc2_CS1, _mc2_INA2, _mc2_INB2, _mc2_EN2DIAG2, _mc2_CS2, _mc2_PWM1, _mc2_PWM2);

Servo Intake;
Servo Claw;
Servo ClawSolenoid;

Encoder encRD(_encRDInt, _encRDDig);
Encoder encLD(_encLDInt, _encLDig);
Encoder encRL(_encRLInt, _encRLDig);
Encoder encLL(_encLLInt, _encLLDig);

Encoder encClaw(_encClawInt, _encClawDig);

SimpleTimer timer;

int clawTimerId;

void setup() {


  // assign public state variables
  _leftSpeed = 0.0;
  _rightSpeed = 0.0;
  _liftSpeed = 0.0;
  _leftLiftPos = 0.0;
  _rightLiftPos = 0.0;   
  _intakeDirection = 0;

  _clawDirection = 0;
  _clawSpeed = 0;


  Serial.begin(115200);
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }

  md1.init();
  md2.init();

  Intake.attach(_intakePWM);
  Claw.attach(_clawPWM);
  ClawSolenoid.attach(_clawSolenoidPWM);

  pinMode(_ledGrn, OUTPUT);
  pinMode(_ledBlu, OUTPUT);
  pinMode(_ledRed, OUTPUT);
 //pinMode(_leftLimitSwitch, INPUT);
  pinMode(_rightLimitSwitch, INPUT);



  timer.setInterval(_readControllerInterval, readController);
  timer.setInterval(_writeControllerInterval, writeController);
  timer.setInterval(_readRobotInterval, readRobot);
  timer.setInterval(_writeRobotInterval, writeRobot);
  timer.setInterval(200, changeColor);
  timer.setInterval(20, runPID);

  clawTimerId = timer.setInterval(500, turnOffClaw);

  armInput = 60;
  armSetpoint = 60; //_ESC_HK_MAX-40;

  driveInput = 0;
  driveSetpoint = 0;

  liftInput = 0;
  liftSetpoint = 0;

  //turn the PID on
  armPID.SetMode(AUTOMATIC);
  armPID.SetOutputLimits(_ESC_HK_MIN+40,_ESC_HK_MAX-40);

  drivePID.SetMode(AUTOMATIC);
  drivePID.SetOutputLimits(-400,400);

  liftPID.SetMode(AUTOMATIC);
  liftPID.SetOutputLimits(-400,400);

  /*if(digitalRead(_rightLimitSwitch) == LOW)
  {
    LiftSpeed(-200);
    delay(2500);
    LiftSpeed(0);
    ClawSolenoid.write(_ESC_HK_MAX-20);
    delay(200);
    ClawSolenoid.write(93);


    //ClawMove(1);
    //delay(800);
    

    for(int i = 0; i < 100; i++)
    {
      Input = encClaw.read();
      myPID.Compute();

      //Serial.println(Output);

      //if(Output > _ESC_HK_MAX-40)
        //Output = _ESC_HK_MAX-40;

      Claw.write(Output);
      delay(50);

  
      
    }
    Claw.write(90);
    LiftSpeed(200);
    delay(3000);
    LiftSpeed(0);
  }*/


}

void runPID()
{
  if(encClaw.read() >= _armMin-20 && encClaw.read() <= _armMax+20)
  {
    armInput = encClaw.read();
    armPID.Compute();
    Claw.write(armOutput);
  }

    driveInput = encLD.read();
    drivePID.Compute();
    //MoveSpeed(driveOutput,driveOutput);

    liftInput = encRL.read();
    liftPID.Compute();
    //LiftSpeed(liftOutput);

}

int red = 0;
int blue = 0;
int green = 0;

void changeColor(){
 
    red = red+10;
    if (red > 255) red = 0;
 
    green = green+15;
    if (green > 255) green = 0;
 
    blue =blue+5;;
    if (blue > 255) blue = 0;
  
  analogWrite(_ledRed, red);
  analogWrite(_ledGrn, green);
  analogWrite(_ledBlu, blue);  
  //delay(10);
}


void loop() {

  timer.run();

}



void readController(){

  


  Usb.Task();

  if (Xbox.XboxReceiverConnected) 
  {
    for (uint8_t i = 0; i < 4; i++) 
    {
      if (Xbox.Xbox360Connected[i]) 
      {
        //Left Hat Y Axis
        if (Xbox.getAnalogHat(LeftHatY, i) > 7500 || Xbox.getAnalogHat(LeftHatY, i) < -7500) {
          _leftSpeed = 400.0 / 32767 * Xbox.getAnalogHat(LeftHatY, i); 
          
        }
        else
        {
          _leftSpeed = 0.0;
          
        }
        //Serial.print(LeftHatY); 
        //Right Hat Y Axis
        if (Xbox.getAnalogHat(RightHatY, i) > 7500 || Xbox.getAnalogHat(RightHatY, i) < -7500) {
          _rightSpeed = 400.0 / 32767 * Xbox.getAnalogHat(RightHatY, i); 
        }
        else
        {
          _rightSpeed = 0.0;
        }
        //L2 Trigger
        if (Xbox.getButtonPress(L2, i)) 
        {
          _liftSpeed = 400.0 / 255 * Xbox.getButtonPress(L2, i) * -1; 
        }
        //R2 Trigger
        else if (Xbox.getButtonPress(R2, i)) 
        {
          _liftSpeed = 400.0 / 255 * Xbox.getButtonPress(R2, i); 
        }
        else
        {
          _liftSpeed = 0.0;
        }

        if (Xbox.getButtonClick(Y, i)) 
        {
              _autoProgNum = 1;
              if(_autoProgNum != 0)
              {
                if(!_autoRunning)
                {
                  // Start Program
                  autoStart();
                } 
                else 
                {
                  // Stop Program
                  autoStop();
                }
              }
          }

        if (Xbox.getButtonPress(L1, i))
        {
          _intakeDirection = 1;
        
        }
        //R1 Button
        else if (Xbox.getButtonPress(R1, i))
        {
          _intakeDirection = -1;
          
        }
        else
        {
          _intakeDirection = 0;
          
        }

        if (Xbox.getButtonPress(LEFT, i)) {
          armSetpoint = _armMin;

        }
        else if (Xbox.getButtonPress(RIGHT, i)) {
          armSetpoint = _armMax;
          
        }
        else
        {
          _clawSpeed = 0;
        }

        if (Xbox.getButtonPress(UP, i)) {
          driveSetpoint = driveSetpoint + 10;

        Serial.println(driveSetpoint);

        }

        if (Xbox.getButtonPress(A, i))
        {
          _clawDirection = 1;
          /*if(_clawDirection == 0)
          {
              _clawDirection = 1;
          }
          else if(_clawDirection == 1)
          {
              _clawDirection = -1;
          }
          else
          {
              _clawDirection = 0;
          }*/
        }
        else
        {
          //Serial.println(_clawDirection);
          if(_clawDirection == 1)
          {
            //Flip back
            Serial.println("Setting to -1");
            _clawDirection = -1;
            timer.enable(clawTimerId);
            
          }
        }
      }
    }
  }
}

void turnOffClaw()
{
  _clawDirection = 0;
  Serial.println("turnClawOff()");
  timer.disable(clawTimerId);
}

void writeController(){

}

void readRobot(){



    
    Serial.print(encRD.read());
    Serial.print("\t");
    Serial.print(encLD.read());
    Serial.print("\t");
    Serial.print(encRL.read());
    Serial.print("\t");
    Serial.print(encLL.read());
    Serial.print("\t");
    Serial.println(encClaw.read());

    


    //Serial.print("\t");
    //Serial.print(digitalRead(_rightLimitSwitch));
    //Serial.println("\t");
    //Serial.print(digitalRead(_leftLimitSwitch));
    //Serial.print("\t");
    //Serial.println(encLL.read());
    //Serial.println(encClaw.read());
   /* Serial.print(Setpoint);
    Serial.print("\t");
    Serial.print(Input);
    Serial.print("\t");
    Serial.println(Output);*/

}

void writeRobot(){
  if (!_autoRunning)
  {
    MoveSpeed(_leftSpeed, _rightSpeed);
    LiftSpeed(_liftSpeed);
    IntakeDirection(_intakeDirection);
    ClawMove(_clawSpeed);
    ClawDirection(_clawDirection);

  } 
  else 
  {
    switch(_autoProgNum)
    {
      case 1: 
        autoRedGoal();
        break;
      case 2:
       // autoBlueGoal();
        break;
      case 3: 
        //autoRedChin();
        break;
      case 4:
        //autoBlueChin();
        break;
    }
    if(!_pauseForPID)
      _autoInterval = _autoInterval + _writeRobotInterval;

    //Safety switch in case forgot to call autoStop
    if(_autoInterval > _maxAutonomousDuration)
    {
      //autoStop();
    }
  }
}

// ===========================================
// AUTONOMOUS METHODS
// ===========================================



// Initialize Autonomous Mode
void autoStop()
{
  _autoRunning = false;
  _autoProgNum = 0;
  _autoInterval = 0;
}

void autoStart()
{
  if(_autoProgNum != 0)
  {
    _autoRunning = true;
    _autoInterval = 0;
  }
}


void autoRedGoal()
{
  
  switch(_autoInterval)
  {
    case 0:
      LiftSpeed(-200);
      break;

    case 2500:
      LiftSpeed(0);
      ClawSolenoid.write(_ESC_HK_MAX-20);
      break;

    case 2700:
      ClawSolenoid.write(93);
      break;

    case 2800:
      _pauseForPID = true;
      armSetpoint = 60; 

      armInput = encClaw.read();
      armPID.Compute();

      Claw.write(armOutput);

      if(armInput > (armSetpoint-5) && armInput < (armSetpoint+5))
        _pauseForPID = false;

      break;

    case 2900:
        Claw.write(90);
        LiftSpeed(200);
      break;

      case 5900:
        LiftSpeed(0);
        break;


    case 6000:
      
      autoStop();
      break;
      
  }
}




// ===========================================
// ROBOT METHODS
// ===========================================

void IntakeDirection(int intakeDirection)
{
  switch(intakeDirection)
  {
    case 1:
      Intake.write(_ESC_HK_MAX);
      //Serial.write("MAX");
      break;
    case -1:
      Intake.write(_ESC_HK_MIN);
      //Serial.write("MIN");
      break;
    case 0:
      Intake.write(93);
      //Serial.write("0");
      break;
  }
}

//This is the solenoid
void ClawDirection(int clawDirection)
{
  switch(clawDirection)
  {
    case 1:
      ClawSolenoid.write(_ESC_HK_MIN+20);
      //Serial.write("MAX");
      break;
    case -1:
      ClawSolenoid.write(_ESC_HK_MAX-20);
      //Serial.write("MIN");
      break;
    case 0:
      ClawSolenoid.write(93);
      //Serial.write("0");
      break;
  }
}

void ClawMove(int moveDirection)
{
  switch(moveDirection)
  {
    case 1:
      Claw.write(_ESC_HK_MAX-40);
      //Serial.write("MAX");
      break;
    case -1:
      Claw.write(_ESC_HK_MIN+40);
      //Serial.write("MIN");
      break;
    case 0:
      Claw.write(93);
      //Serial.write("0");
      break;
  }
}

 void MoveSpeed(float leftSpeed, float rightSpeed)
 {
  //Set Left Speed
  md2.setM2Speed(leftSpeed);

  //Set Right Speed
  md1.setM1Speed(rightSpeed);
  //var asdf = "asf";

 }

 void LiftSpeed(float liftSpeed)
 {
    //Set Lift Speed, Brake if Zero
    if(liftSpeed > -25 && liftSpeed < 25)
    {
      md2.setM1Brake(400);
      md1.setM2Brake(400);

        //Set Left Speed
      //md2.setM2Speed(_leftSpeed);

      //Set Right Speed
     //md2.setM1Speed(_rightSpeed);
    }
    else
    {
      md2.setM1Speed(liftSpeed);
      md1.setM2Speed(liftSpeed);
    }
 }
