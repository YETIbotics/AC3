/*
 * SimpleTimer.h
 *
 * SimpleTimer - A timer library for Arduino.
 * Author: mromani@ottotecnica.com
 * Copyright (c) 2010 OTTOTECNICA Italy
 *
 * This library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * This library is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser
 * General Public License along with this library; if not,
 * write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * EDIT 19-01-2012
 * Author: info@aduen.nl
 *
 * Removed all the arrays execpt for one and made it into an array of structs
 *
 * Removed the function pointer and replaced it with a delegate
 *
 * Fixed bug, defining short delays and long delays caused problems because the
 * numTimers would just decrease although there was still a running timer in that
 * part of the array.
 *
 * Added the getAvailableSpot function to overcome this problem and made the getNumTimers
 * dynamic.
 *
 * Added an int as callback function parameter, it either can be used to know which timer is calling
 * the function or you can use the overloaded setTimer function to add your own paramter.
 */

#ifndef SIMPLETIMER_H
#define SIMPLETIMER_H

#include <Arduino.h>
#include "FastDelegate.h"

using namespace fastdelegate;

typedef FastDelegate0<void> timer_delegate;

typedef struct {
	// pointer to the callback functions
	timer_delegate callback;
	// delay values
	long unsigned int delay;
	// value returned by the millis() function
	// in the previous run() call
	long unsigned int prev_millis;
	// number of runs to be executed for each timer
	int maxRuns;
	// number of executed runs for each timer
	int numRuns;
	// optional parameter for callback function, default timer_id is used
	int params;
	// which timers are enabled
	bool enabled;
} timer_call;

class SimpleTimer {

public:
    // maximum number of timers
    const static int MAX_TIMERS = 10;

    // setTimer() constants
    const static int RUN_FOREVER = 0;
    const static int RUN_ONCE = 1;

    // constructor
    SimpleTimer();

    // this function must be called inside loop()
    void run();

    // call function f every d milliseconds
    int setInterval(long d, timer_delegate f);

    // call function f once after d milliseconds
    int setTimeout(long d, timer_delegate f);

    // call function f every d milliseconds for n times
    int setTimer(long d, timer_delegate f, int n, int param);
    int setTimer(long d, timer_delegate f, int n);

    // destroy the specified timer
    void deleteTimer(int numTimer);

    // returns true if the specified timer is enabled
    boolean isEnabled(int numTimer);

    // enables the specified timer
    void enable(int numTimer);

    // disables the specified timer
    void disable(int numTimer);

    // enables or disables the specified timer
    // based on its current state
    void toggle(int numTimer);

    // returns the number of used timers
    int getNumTimers();

private:
    // in the previous run() call
    timer_call calls[MAX_TIMERS];

    int num_timers;

    int getAvailableSpot();
};

#endif
