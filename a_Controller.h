#ifndef Controller_h
#define Controller_h

#include <Arduino.h>
#include "XBOXRECV.h"


class Controller
{

    private:
    USB *_pUsb;

  public:  
    // CONSTRUCTORS
    Controller(USB *pUsb); // Default pin selection.

   


  

};

#endif