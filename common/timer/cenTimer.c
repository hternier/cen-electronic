#include "cenTimer.h"

#include "../../common/commons.h"

#include "../../common/io/outputStream.h"
#include "../../common/io/printWriter.h"

unsigned long getTime(Timer* timer) {
    return timer->time;
}

void setTime(Timer* timer, unsigned long value) {
    timer->time = value;
}

// STARTS / STOP

void startTimer(Timer* timer) {
    timer->enabled = TRUE;
}

void stopTimer(Timer* timer) {
    timer->enabled = FALSE;
}

// MARK

void mark(Timer* timer) {
	timer->markTime = timer->time;
}

BOOL timeout(Timer* timer, unsigned long time) {
	return timer->markTime + time > timer->time;
}

// LOCK / UNLOCK

void lockAndWaitForTimer(Timer* timer) {
    // we lock the timer to be sure that he will not be fired
    timer->lock = TRUE;

    // we wait after the end of the timer callback
    // do be sure that variables are ok
    while (timer->working) {
        // do nothing
    }
}

void unlockTimer(Timer* timer) {
    timer->lock = FALSE;
}
