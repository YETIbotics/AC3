// INCLUDES  24
#include <XBOXRECV.h>
#include <DualVNH5019MotorShield.h>
#include <SimpleTimer.h>

#include <Encoder.h>



//#ifdef dobogusinclude
//#include <spi4teensy3.h>
//#endif

// PRIVATE STATE VARIABLES
float _leftSpeed;
float _rightSpeed; 
float _liftSpeed;
int _intakeDirection;
bool _isThrowing; 

bool _autoRunning;
int _autoProgNum;
int _autoInterval;

int _maxAmps;
int _totalAmps;
long _encLeftPos;
long _encRightPos;
long _encLiftPos;

bool _led01HIGH;
bool _led02HIGH;

bool _useLiftEncoder;
int _liftEncMax = 50;
int _liftEncMin = 0;
int _liftEncGoal = 41;

bool _chinupMode;
bool _liftBraked;
bool _throttlePrecision;

// PUBLIC CONSTANT VALUES
// Enable/Disable Serial Output
const bool _serial = true;

// Motor Controller 1 Pinouts 
const int _mc1_INA1 = 26;
const int _mc1_INB1 = 28;
const int _mc1_EN1DIAG1 = 22;
const int _mc1_CS1 = A3;
const int _mc1_INA2 = 24;
const int _mc1_INB2 = 30;
const int _mc1_EN1DIAG2 = 32;
const int _mc1_CS2 = A2; 
const int _mc1_PWM1 = 2;
const int _mc1_PWM2 = 3;

// Motor Controller 2 Pinouts
const int _mc2_INA1 = 27;
const int _mc2_INB1 = 29;
const int _mc2_EN1DIAG1 = 23;
const int _mc2_CS1 = A5;
const int _mc2_INA2 = 25;
const int _mc2_INB2 = 31;
const int _mc2_EN1DIAG2 = 33;
const int _mc2_CS2 = A4; 
const int _mc2_PWM1 = 4;
const int _mc2_PWM2 = 5;

//Encoder Left Pinouts
const int _encLeftInt = 19;
const int _encLeftDig = 39;

//Encoder Right Pinouts
const int _encRightInt = 21;
const int _encRightDig = 35;

//Encoder LIFT Pinouts
const int _encLiftInt = 20;
const int _encLiftDig = 37;

//Lift Brake
//const int _liftBrake = 34;

//LED Pinouts
const int _led01 = 41;
const int _led02 = 43;

//RESET Digital Pin
const int _watchDogPat = 6;

//USB Pinouts
//Are Static in Library
/*
const int _usb1 = 47; //Digital 2
const int _usb1 = 49; //Digital 1
const int _usb1 = 50; //MISO
const int _usb1 = 51; //MOSI
const int _usb1 = 52; //SCK
*/

// Intake motors pinouts
//const int _intake1 = 34;
//const int _intake2 = 35;

bool _controllerConnected;
bool _watchdogInitialized;


// Timer Polling Intervals
const int _readControllerInterval = 10;
const int _writeControllerInterval = 15;
const int _writeRobotInterval = 20;
const int _readRobotInterval = 25;
const int _heartbeatInterval = 750;

// Autonomous Timeout Duration (milliseconds)
const int _maxAutonomousDuration = 60000;

//PUBLIC VARIABLES
USB Usb;
XBOXRECV Xbox(&Usb);
DualVNH5019MotorShield md1(_mc1_INA1, _mc1_INB1, _mc1_EN1DIAG1
  , _mc1_CS1, _mc1_INA2, _mc1_INB2, _mc1_EN1DIAG2, _mc1_CS2, _mc1_PWM1, _mc1_PWM2);
DualVNH5019MotorShield md2(_mc2_INA1, _mc2_INB1, _mc2_EN1DIAG1
  , _mc2_CS1, _mc2_INA2, _mc2_INB2, _mc2_EN1DIAG2, _mc2_CS2, _mc2_PWM1, _mc2_PWM2);

Encoder encLeft(_encLeftInt, _encLeftDig);
Encoder encRight(_encRightInt, _encRightDig);
Encoder encLift(_encLiftInt, _encLiftDig);

//Servo
//Servo brake;

//http://playground.arduino.cc/Code/SimpleTimer
SimpleTimer timer;


void setup() 
{
  //have watchdog enabled, reset & disable so that it doesn't loop
  //wdt_reset();
  //wdt_disable();


  // assign public state variables
  _leftSpeed = 0.0;
  _rightSpeed = 0.0;
  _liftSpeed = 0.0;
  _intakeDirection = 0;
  _isThrowing = false;

  _autoRunning = false;
  _autoProgNum = 0; 
  _autoInterval = 0;

  _totalAmps = 0;
  _maxAmps = 0;
  _encLeftPos = 0;
  _encRightPos = 0;
  _encLiftPos = 0;

  _led01HIGH = true;
  _led02HIGH = true;

  _controllerConnected = false;
  _watchdogInitialized = false;
  _liftBraked = false;

  _useLiftEncoder = true;
  _chinupMode = false;
  _throttlePrecision = true;

  //intake1.attach(_intake1);
  //intake2.attach(_intake2);
  //brake.attach(_liftBrake);
  //Set status LED pins to OUTPUTS and set to On
  pinMode(_led01, OUTPUT);
  pinMode(_led02, OUTPUT);
  digitalWrite(_led01, _led01HIGH);
  digitalWrite(_led01, _led01HIGH);

  //Set Brake Pin To Output
  /*pinMode(_liftBrake, OUTPUT);
  digitalWrite(_liftBrake, LOW);*/

  

  if(_serial)
  {
    Serial.begin(115200);
      while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
    }

  logMsg("Starting Xbox Wireless Receiver Library...");
    if (Usb.Init() == -1) 
    {
      logMsg("USB did not start!");
      while (1); //halt
  }
  logMsg("Xbox Wireless Receiver Library Started");

  logMsg("Initializing Dual VNH5019 Motor Shield 1..."); 
  md1.init();
  logMsg("Dual VNH5019 Motor Shield 1 Initialized"); 

  logMsg("Initializing Dual VNH5019 Motor Shield 2..."); 
  md2.init();
  logMsg("Dual VNH5019 Motor Shield 2 Initialized"); 

  logMsg("Initializing Timers...");
  timer.setInterval(_readControllerInterval, readController);
  timer.setInterval(_writeControllerInterval, writeController);
  timer.setInterval(_readRobotInterval, readRobot);
  timer.setInterval(_writeRobotInterval, writeRobot);
  timer.setInterval(_heartbeatInterval, heartBeat);
  logMsg("Timers Initialized");

  
  //ResetLift();

}

//This might not work...
void logMsg(String msg)
{
  if(_serial)
  {
    Serial.println("\r\n :: " + msg);
  }
}

//This Doesn't work
//void(* resetFunc) (void) = 0; //declare reset function @ address 0

//This Doesn't work either
//void osftware_Reset()
//{
//  asm volatile ("  jmp 0");
//}

void loop() 
{
  // put your main code here, to run repeatedly: 
  timer.run();
  
}

uint32_t shutOffRumble;
void heartBeat()
{
  if(shutOffRumble > millis())
  {
    Xbox.setRumbleOff();
    shutOffRumble = 0;
  }
  // This gets set to true when the controller intially connects.. No sense in heartbeating if there is not a controller
  // connected in the first place
  if(_controllerConnected)
  {
    if(!_watchdogInitialized)
    {
      //Start the WatchDog!
      //Moved the wdt enable to heartbeat because it needs to wait until the USB inits
      Xbox.setRumbleOn(0,255,0);
      shutOffRumble = millis() + 1000;

      //This pin is attached to UNO watchdog
      digitalWrite(_watchDogPat, HIGH);
      pinMode(_watchDogPat, OUTPUT);
      

      _watchdogInitialized = true;
    }
    else
    {
      if ((!Xbox.XboxReceiverConnected || !Xbox.Xbox360Connected[0]))
      {
          //_controllerConnected = false;
            Serial.println("XBOX DISCONNECTED (heartbeat)");

          
            //osftware_Reset();
          //resetFunc();

          
        }
        else
        {
          digitalWrite(_watchDogPat, _led01HIGH);

        digitalWrite(_led01, _led01HIGH);
        _led01HIGH = !_led01HIGH;
          //wdt_reset();
          //Serial.println("Heartbeat");

          
        }
    }
  }
}

void readRobot()
{
  _totalAmps = md1.getM1CurrentMilliamps() + md1.getM2CurrentMilliamps();
    _totalAmps += md2.getM1CurrentMilliamps() + md2.getM2CurrentMilliamps();
    
    _totalAmps = _totalAmps / 1000 / 2;




    if(_totalAmps > _maxAmps)
          {
            _maxAmps = _totalAmps;
            
          }
    //Serial.println(amps);


  if(_encLeftPos != -1* encLeft.read() || 
    _encRightPos != encRight.read() || 
    (_encLiftPos) != encLift.read() )
  {
    Serial.print("LeftEnc: ");
      Serial.print(_encLeftPos);
      Serial.print("\t");
    Serial.print("RightEnc: ");
      Serial.print(_encRightPos);
      Serial.print("\t");
    Serial.print("LIFT Enc: ");
      Serial.print(_encLiftPos);
      Serial.print("\t");

      Serial.println();

      digitalWrite(_led02, _led02HIGH);
    _led02HIGH = !_led02HIGH;
  }

    //Read Encoers and update position
    _encLeftPos = -1* encLeft.read();
  _encRightPos = encRight.read();
  _encLiftPos = encLift.read();
}

void readController()
{
  Usb.Task();

  if (Xbox.XboxReceiverConnected) 
  {
      for (uint8_t i = 0; i < 4; i++) 
      {
          if (Xbox.Xbox360Connected[i]) 
          {
            _controllerConnected = true;

            //A button
             if (Xbox.getButtonClick(Y, i))
             {
                _liftBraked = !_liftBraked;
             }

            //Pressing this button will reset lift and toggle into encoder mode.
            if (Xbox.getButtonClick(BACK, i)) 
            {
              _useLiftEncoder = !_useLiftEncoder;
              //Reset to zero (makes the assumption that lift is all the way down.)
              if(_useLiftEncoder)
              {
                encLift.write(0);
              }
            }



            if (Xbox.getButtonClick(X, i)) 
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
          if (Xbox.getButtonClick(B, i)) 
            {
              _throttlePrecision = !_throttlePrecision;
            }

            if (Xbox.getButtonClick(A, i)) 
            {
              _chinupMode = !_chinupMode;
            }
            
            //Left Hat Y Axis
            if (Xbox.getAnalogHat(LeftHatY, i) > 7500 || Xbox.getAnalogHat(LeftHatY, i) < -7500) {
              _leftSpeed = 400.0 / 32767 * Xbox.getAnalogHat(LeftHatY, i); 
              
            }
            else
            {
              _leftSpeed = 0.0;
              
            }

            //Right Hat Y Axis
            if (Xbox.getAnalogHat(RightHatY, i) > 7500 || Xbox.getAnalogHat(RightHatY, i) < -7500) {
              _rightSpeed = 400.0 / 32767 * Xbox.getAnalogHat(RightHatY, i); 
            }
            else
            {
              _rightSpeed = 0.0;
            }

            //D-PAD Controls 
             if (Xbox.getButtonPress(DOWN, i)) 
              {
                  _leftSpeed = -325;
                  _rightSpeed = -325;
              }

           if (Xbox.getButtonPress(UP, i)) 
              {
                  _leftSpeed = 325;
                  _rightSpeed = 325;
              }

            //Left Hat Click
            if (Xbox.getButtonClick(L3, i))
            {
              //Throw();
            }

            //Right Hat Click
            if (Xbox.getButtonClick(R3, i))
            {

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

        //L1 Button
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

        // Start Button 
        if(Xbox.getButtonClick(START, i))
        {
          if(!_autoRunning) 
          {
            if(_autoProgNum != 4) 
            {
              _autoProgNum++;
            } 
            else 
            {
              _autoProgNum = 0;
            }
          } 
        }

        // Back Button
        /*if(Xbox.getButtonClick(BACK, i))
        {
          if(!_autoRunning) 
          {
            _autoProgNum = 0;
          }
        }*/

        // XBox Button
        if(Xbox.getButtonClick(XBOX, i))
        {
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

        
          }
        }
    }
}

void writeController()
{
  if (Xbox.XboxReceiverConnected) 
  {
      for (uint8_t i = 0; i < 4; i++) 
      {
      
      //Make sure were not in auto program mode
      if(_autoProgNum == 0)
      {

        if(_totalAmps > 1 && _totalAmps <= 2)
          {
             Xbox.setLedOn(LED1, i);
          }
          
          if(_totalAmps > 2 && _totalAmps <= 3)
          {
             Xbox.setLedOn(LED2, i);
          }
          
          if(_totalAmps > 3 && _totalAmps <= 4)
          {
             Xbox.setLedOn(LED3, i);
          }
          
          if(_totalAmps > 4)
          {
             Xbox.setLedOn(LED4, i);
          }
          
          if(_totalAmps < 1)
          {
            Xbox.setLedOn(OFF, i);
          }
          
          uint8_t rmbl = 0;
          if(_totalAmps > 4)
          {
            rmbl = 255.0 / 6 * (_totalAmps-4);
          }
      }
      else
      {
        switch(_autoProgNum)
        {
          case 1: 
            Xbox.setLedOn(LED1, i);
            break;
          case 2:
            Xbox.setLedOn(LED2, i);
            break;
          case 3: 
            Xbox.setLedOn(LED3, i);
            break;
          case 4:
            Xbox.setLedOn(LED4, i);
            break;
        }
      }
    }
  }
}

void writeRobot()
{
  if (!_autoRunning)
  {
    MoveSpeed(_leftSpeed, _rightSpeed);
    LiftSpeed(_liftSpeed);
    MoveIntake(_intakeDirection);
    
    //digitalWrite(_liftBrake, _liftBraked);
    /*if (_liftBraked) 
    {
      brake.write(130);
    } 
    else
    {
      brake.write(90); 
    }*/


  } 
  else 
  {
    switch(_autoProgNum)
    {
      case 1: 
        autoRedGoal();
        break;
      case 2:
        autoBlueGoal();
        break;
      case 3: 
        autoRedChin();
        break;
      case 4:
        autoBlueChin();
        break;
    }
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

// XBox Button 4
void autoBlueChin()
{
  switch(_autoInterval)
  {
    case 0:
      MoveSpeed(400, 400);
      break;

    case 2000:
      MoveSpeed(400, -400);
      break;

    case 3500:
      MoveSpeed(400, 400);
      break;

    case 5000:
      MoveSpeed(-400, 400);
      break;

    case 6000:
      MoveSpeed(400, 400);
      break;

    case 7500:
      MoveSpeed(-400, 400);
      break;

    case 8000:
      MoveSpeed(-400, -400);
      break;

    case 10000:
      MoveSpeed(0, 0);
      autoStop();
      break;
      
  }
}
// XBox Button 3
void autoRedChin()
{
  int oneSquare = 2000;
  int leftSpeed = 300;
  int rightSpeed = 300;
  switch(_autoInterval)
  {
    case 0:
      _useLiftEncoder = false;
      LiftSpeed(-400);
      MoveSpeed(leftSpeed, rightSpeed);

      MoveIntake(-1);
      break;
    
    case 1000:
      LiftSpeed(0);
      MoveSpeed(0, 0);
      
      break;

    case 1100: //first backup
      MoveSpeed(-leftSpeed, -rightSpeed);

      break;

    case 1700: //1st turn
      MoveSpeed(leftSpeed, -rightSpeed);
      break;

    case 1900: //hit second ball
      MoveSpeed(leftSpeed, rightSpeed);
      break;

    case 2880: //2nd back up
      MoveSpeed(-leftSpeed, -rightSpeed);
      break;  

    case 3580: // 2ndturn
      MoveSpeed(leftSpeed, -rightSpeed);
      LiftSpeed(-400);
      break;

    case 3720: // hit third ball
      MoveSpeed(leftSpeed, rightSpeed);
      LiftSpeed(0);
      break;
      ///////////
    case 6860: // 3rd back up
      MoveSpeed(-leftSpeed, -rightSpeed);
      break;  

    case 7560: // 3rdturn
      MoveSpeed(leftSpeed, -rightSpeed);
      LiftSpeed(-400);
      break;

    case 7620: // hit third ball
      MoveSpeed(leftSpeed, rightSpeed);
      LiftSpeed(0);
      break;

    case 9500: 
      MoveSpeed(-leftSpeed, rightSpeed);

      break;

    case 10620:
      MoveSpeed(0,0);
      LiftSpeed(0);
      MoveIntake(0);
      autoStop();
      break;

  }

}
//XBox Button 2
void autoBlueGoal()
{
  switch(_autoInterval) 
  {
    case 0:
      MoveSpeed(200, 200);
      MoveIntake(1);
      break;

    
    case 2400:
      MoveSpeed(-400, -400);
      MoveIntake(0);
      break;
    

    case 2800:
      MoveSpeed(-400, 400);
      break;

    case 3640:
      MoveSpeed(400, 400);
      LiftSpeed(-200);
      break;

    case 4100:
      MoveSpeed(400, 400);
      LiftSpeed(0);
      MoveIntake(-1);
      break;

    case 4980:
      MoveSpeed(0,0);
      LiftSpeed(-400);
      break;

    case 7100:
      MoveSpeed(400, -400);
      LiftSpeed(0);
      break;
    case 8100:
      MoveSpeed(0,0);
      autoStop();
      break;
  }
}
//XBox Button 1
void autoRedGoal()
{
  
  switch(_autoInterval)
  {
    case 0:
      _useLiftEncoder = false;
      _chinupMode = false;
      MoveSpeed(-200, -200);
      break;

    case 1000:
      
      MoveSpeed(-200, -200);
      LiftSpeed(400);
      break;
    
    case 3000:
      MoveSpeed(0, 0);
      break;

    case 5000:
      LiftSpeed(0);
      break;

    case 6000:
      LiftSpeed(400);
      break;
    
    case 9000:
      MoveSpeed(0, 0);
      LiftSpeed(0);
      autoStop();
      _useLiftEncoder = true;
      break;



  }
}





// ===========================================
// ROBOT METHODS
// ===========================================

 void MoveSpeed(float leftSpeed, float rightSpeed)
 {
  

  if (_chinupMode)
  {
    float preMax = 150;
    //float rMax = -4 * _encLiftPos + 400;

    

    leftSpeed = map(_leftSpeed, 0, 400, 0, preMax);
    rightSpeed = map(_rightSpeed, 0, 400, 0, preMax);
  }
  else 
  {
    if(_intakeDirection == 1)
    {
      float preMax = 300;
      //float rMax = -4 * _encLiftPos + 400;

      

      leftSpeed = map(_leftSpeed, 0, 400, 0, preMax);
      rightSpeed = map(_rightSpeed, 0, 400, 0, preMax);
    }
    else
    {
      if(_useLiftEncoder && _throttlePrecision)
      {
        float preMax = -4 * _encLiftPos + 400;
        //float rMax = -4 * _encLiftPos + 400;

        

        leftSpeed = map(_leftSpeed, 0, 400, 0, preMax);
        rightSpeed = map(_rightSpeed, 0, 400, 0, preMax);
      }
    }
  }
  //Set Right Speed
  md1.setM1Speed(-1 * leftSpeed);

  //Set Left Speed
  md1.setM2Speed(-1 * rightSpeed);
  //Serial.println(leftSpeed);
    //Serial.println(_rightSpeed);

  //if (rightSpeed == 0 && leftSpeed == 0)
  //{
  //  md1.setM2Brake(400);
  //  md1.setM1Brake(400);
  //}
 }

 void MoveTime(float leftSpeed, float rightSpeed, long duration)
 {

 }

 void LiftSpeed(float liftSpeed)
 {
  //Serial.println(liftSpeed);
  if(_useLiftEncoder)
  {
    if(_chinupMode)
    {
      if(_encLiftPos >= _liftEncMax  && liftSpeed < 0)
      {
        md2.setM1Brake(400);
        md2.setM2Brake(400);
      }
      else if(_encLiftPos <= (_liftEncMin+3) && liftSpeed > 0)
      {
        md2.setM1Brake(400);
        md2.setM2Brake(400);
      }
      else
      {
        //Set Lift Speed, Brake if Zero
        if(liftSpeed > -25 && liftSpeed < 25)
        {
          md2.setM2Brake(400);
        }
        else
        {
          if(_encLiftPos > 100 || _encLiftPos < -10)
          {
            liftSpeed = liftSpeed * .5;
          }
          else if(_encLiftPos > 100 || _encLiftPos < -10)
          {
            liftSpeed = liftSpeed * .65;
          }
          else if(_encLiftPos > 100 || _encLiftPos < -10)
          {
            liftSpeed = liftSpeed * .85;
          }
          //Serial.println(liftSpeed);
          md2.setM2Speed(liftSpeed);
        }
      }
    }
    else
    {
      if(_encLiftPos >= _liftEncGoal  && liftSpeed < 0)
      {
        md2.setM1Brake(400);
        md2.setM2Brake(400);
      }
      else if(_encLiftPos <= (_liftEncMin+3) && liftSpeed > 0)
      {
        md2.setM1Brake(400);
        md2.setM2Brake(400);
      }
      else
      {
        //Set Lift Speed, Brake if Zero
        if(liftSpeed > -25 && liftSpeed < 25)
        {
          md2.setM2Brake(400);
        }
        else
        {
          if(_encLiftPos > 100 || _encLiftPos < 5)
          {
            liftSpeed = liftSpeed * .45;
          }
          else if(_encLiftPos > 100 || _encLiftPos < 10)
          {
            liftSpeed = liftSpeed * .65;
          }
          else if(_encLiftPos > 100 || _encLiftPos < 15)
          {
            liftSpeed = liftSpeed * .85;
          }
          //Serial.println(liftSpeed);

          md2.setM2Speed(liftSpeed);
        }
      }
    }
  }
  else
  {
    //Set Lift Speed, Brake if Zero
    if(liftSpeed > -25 && liftSpeed < 25)
    {
      md2.setM2Brake(400);
    }
    else
    {
      md2.setM2Speed(liftSpeed);
    }
  }
  
  
  //Serial.println(liftSpeed);
 }

 void ResetLift()
 {
  long oldLiftPos = 1;

  md2.setM2Speed(150);

  while(oldLiftPos != encLift.read())
  {
    oldLiftPos = encLift.read();
    delay(200);
  }

  md2.setM2Brake(400);

  encLift.write(0);
 }

 void LiftTime(float liftSpeed, long duration)
 {
  
 }

 void LiftTo(int position, float liftSpeed)
 {
  
 }

 void MoveIntake(int inPosition)
 {
  switch(inPosition)
  {
    case 1:
      //intake1.write(150);
            //intake2.write(150);
      md2.setM1Speed(-300);
      break;
    case -1:
      //intake1.write(30);
            //intake2.write(30);
      md2.setM1Speed(300);
      break;
    case 0:
      //intake1.write(90);
            //intake2.write(90);
      md2.setM1Speed(0);
      break;
  }
 }

 void IntakeTime(long duration)
 {
  
 }

 void Throw()
 {/*
  if (_isThrowing)
  {
    md2.setM1Speed(-400);
    delay(100);
    md2.setM1Speed(0);
    _isThrowing = false;
  }
  else 
  {
    md2.setM1Speed(400);
    delay(400);
    md2.setM1Speed(0);
    _isThrowing = true;
  }*/
 }






