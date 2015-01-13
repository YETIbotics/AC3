#include "a_Controller.h"

// Constructors ////////////////////////////////////////////////////////////////

    XBOXRECV Xbox(&_pUsb);

Controller::Controller(USB *p)
{
	_pUsb = p;
}