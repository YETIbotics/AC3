#ifndef Controller_h
#define Controller_h

#include <Arduino.h>
#include "XBOXRECV.h"


class Controller
{
  public:  
    // CONSTRUCTORS
    Controller(USB *pUsb); // Default pin selection.

    void Task();

    XBOXRECV Xbox;

    float LeftJoystick;

   private:

};

#endif