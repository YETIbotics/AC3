/*
 * SimpleTimer.cpp
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

#include "SimpleTimer.h"

SimpleTimer::SimpleTimer() {
    long current_millis = millis();

    for (int i = 0; i < MAX_TIMERS; i++) {
        calls[i].enabled = false;
        calls[i].callback = 0;
        calls[i].prev_millis = current_millis;
        calls[i].params = -1;
    }

    num_timers = 0;
}


void SimpleTimer::run() {
    int i;
    long current_millis;

    // get current time
    current_millis = millis();

    for (i = 0; i < MAX_TIMERS; i++) {

        // only process active timers
        if (calls[i].callback && calls[i].enabled) {

            // is it time to process this timer ?
            if (current_millis - calls[i].prev_millis >= calls[i].delay) {

                // update time
            	calls[i].prev_millis = current_millis;

                // "run forever" timers must always be executed
                if (calls[i].maxRuns == RUN_FOREVER) {
                    (calls[i].callback)();
                }
                // other timers get executed the specified number of times

                //BUG take better care of your memory
                //if an earlier short timer gets deleted before an long later one, it gets overriden because
                //of the num_timers just stacks upwards and doesnt look down

                else if (calls[i].numRuns < calls[i].maxRuns) {
                    (calls[i].callback)();
                    calls[i].numRuns++;

                    // after the last run, delete the timer
                    // to save some cycles
                    if (calls[i].numRuns >= calls[i].maxRuns) {
                        deleteTimer(i);
                    }
                }
            }
        }
    }
}


int SimpleTimer::setTimer(long d, timer_delegate f, int n) {
    if (num_timers >= MAX_TIMERS) {
        return -1;
    }

    int free_spot = getAvailableSpot();
    if(free_spot < 0)return -1;

    calls[free_spot].delay = d;
    calls[free_spot].callback = f;
    calls[free_spot].maxRuns = n;
    calls[free_spot].enabled = true;
    calls[free_spot].numRuns = 0;
    calls[free_spot].prev_millis = millis();
    calls[free_spot].params = free_spot + num_timers;

    num_timers = getNumTimers();

    return (free_spot + num_timers);
}

int SimpleTimer::setTimer(long d, timer_delegate f, int n, int param) {
    if (num_timers >= MAX_TIMERS) {
        return -1;
    }

    int free_spot = getAvailableSpot();
    if(free_spot < 0)return -1;

    calls[free_spot].delay = d;
    calls[free_spot].callback = f;
    calls[free_spot].maxRuns = n;
    calls[free_spot].enabled = true;
    calls[free_spot].numRuns = 0;
    calls[free_spot].prev_millis = millis();
    calls[free_spot].params = param;

    num_timers = getNumTimers();

    return (free_spot + num_timers);
}


int SimpleTimer::setInterval(long d, timer_delegate f) {
    return setTimer(d, f, RUN_FOREVER);
}


int SimpleTimer::setTimeout(long d, timer_delegate f) {
    return setTimer(d, f, RUN_ONCE);
}


void SimpleTimer::deleteTimer(int numTimer) {
    if (numTimer >= MAX_TIMERS) {
        return;
    }

    // nothing to disable if no timers are in use
    if (num_timers == 0) {
        return;
    }

    calls[numTimer].callback = 0;
    calls[numTimer].enabled = false;
    calls[numTimer].delay = 0;

    num_timers = getNumTimers();
}


boolean SimpleTimer::isEnabled(int numTimer) {
    if (numTimer >= MAX_TIMERS) {
        return false;
    }

    return calls[numTimer].enabled;
}


void SimpleTimer::enable(int numTimer) {
    if (numTimer >= MAX_TIMERS) {
        return;
    }

    calls[numTimer].enabled = true;
}


void SimpleTimer::disable(int numTimer) {
    if (numTimer >= MAX_TIMERS) {
        return;
    }

    calls[numTimer].enabled = false;
}


void SimpleTimer::toggle(int numTimer) {
    if (numTimer >= MAX_TIMERS) {
        return;
    }

    calls[numTimer].enabled = !calls[numTimer].enabled;
}

int SimpleTimer::getAvailableSpot() {
	for (int i = 0; i < MAX_TIMERS; i++) {
		if(calls[i].callback == 0)return i;
	}
	return -1;
}

int SimpleTimer::getNumTimers() {
    int numTimers = 0;
    for (int i = 0; i < MAX_TIMERS; i++) {
		if(calls[i].enabled)numTimers++;
	}
    return numTimers;
}
