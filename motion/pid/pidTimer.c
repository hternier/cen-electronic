#include <stdlib.h>

#include "pidTimer.h"

#include "../../common/commons.h"

#include "../../common/io/outputStream.h"
#include "../../common/io/printWriter.h"

#include "../../common/log/logger.h"
#include "../../common/log/logLevel.h"

#include "../../common/timer/cenTimer.h"
#include "../../common/timer/timerList.h"

// TIMER

static long lastPidTime;

void initPidTimer(void) {
    addTimer(TIMER_PID_CODE, PID_UPDATE_MOTORS_FREQUENCY, (interruptTimerCallbackFunc*) NULL);
    clearPidTime();
}

long getPidTime(void) {
    Timer* timer = getTimerByCode(TIMER_PID_CODE);
    long result = timer->time;
    return result;
}

// TIMERS

void clearPidTime(void) {
    Timer* timer = getTimerByCode(TIMER_PID_CODE);
    timer->time = 0;
    lastPidTime = 0;
}

BOOL mustPidBeRecomputed(void) {
    long pidTime = getPidTime();
    if (pidTime != lastPidTime) {
        lastPidTime = pidTime;
        return TRUE;
    }
    return FALSE;
}
