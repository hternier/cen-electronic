#include <math.h>
#include <stdlib.h>

#include "pid.h"
#include "pidComputer.h"
#include "pidPersistence.h"
#include "pidDebug.h"
#include "motionEndDetection.h"
#include "pidTimer.h"

// Commons

#include "pidTimer.h"

#include "../../common/commons.h"

#include "../../common/error/error.h"

#include "../../common/math/cenMath.h"

#include "../../common/io/buffer.h"
#include "../../common/io/outputStream.h"
#include "../../common/io/printWriter.h"
#include "../../common/io/reader.h"
#include "../../common/io/stream.h"

#include "../../common/log/logger.h"
#include "../../common/log/logLevel.h"

#include "../../device/motor/pwmMotor.h"

#include "../extended/bspline.h"

#include "../position/coders.h"
#include "../position/coderErrorComputer.h"
#include "../simple/motion.h"

#include "../../robot/robotConstants.h"

// Contains all information about current motion
static PidMotion pidMotion;

static char mustReachPosition;

/** Indicates if we use the testing Board (we take lower values for PID). */
static unsigned char rollingTestMode = ROLLING_BOARD_TEST_MODE_OFF;

unsigned char getIndexOfPid(unsigned char instructionIndex, unsigned char pidType) {
    return pidType * INSTRUCTION_COUNT + instructionIndex;
}

unsigned char getRollingTestMode() {
    return rollingTestMode;
}

int getMustReachPosition(void) {
    return mustReachPosition;
}

void setMustReachPosition(int value) {
    mustReachPosition = value;
}

void setEnabledPid(int pidIndex, unsigned char enabled) {
    Pid * localPid = getPID(pidIndex, rollingTestMode);
    localPid->enabled = enabled;
}

PidMotion* getPidMotion() {
	return &pidMotion;
}

MotionEndDetectionParameter* getMotionEndDetectionParameter() {
	return &(pidMotion.globalParameters.motionEndDetectionParameter);
}

/**
 * Init global variable storing information about motion.
 */
void initPidMotion() {
	initMotionEndParameter(getMotionEndDetectionParameter());
	initFirstTimeBSplineCurve(&(pidMotion.currentMotionDefinition.curve));
}

void initPID(void) {
    // TODO : A Remettre
    // rollingTestMode = (getConfigValue() & CONFIG_ROLLING_TEST_MASK) != 0;
    rollingTestMode = 0;
    loadPID();
    initPidTimer();
    initPidMotion();
}

void stopPID(void) {
    mustReachPosition = FALSE;
}

/**
 * Returns the tension which must be applied to the motor to reach normalSpeed, with no load on motor.
 */
float getNormalU(float pulseAtSpeed) {
    // at full Speed (value = 127), 7 rotations / seconds * 20000 impulsions
	// at Frequency of 200 Hz => 730 pulses by pidTime at full Speed
	
	// NormalU = (pulseAtSpeed / pulseAtFullSpeed) * MAX_U
    float result = pulseAtSpeed * U_FACTOR_AT_FULL_SPEED;
	// float result = 0.0f;
    return result;
}

void setPID(int pidIndex, float p, float i, float d, float maxIntegral) {
    Pid* localPid = getPID(pidIndex, rollingTestMode);
    localPid->p = p;
    localPid->i = i;
    localPid->d = d;
    localPid->maxIntegral = maxIntegral;
}

Pid* getPID(int index, unsigned int pidMode) {
    Pid* result = &(pidMotion.globalParameters.pid[index]);
    return result;
}

unsigned int updateMotors(void) {
    if (!mustReachPosition) {
        return NO_POSITION_TO_REACH;
    }
	if (mustPidBeRecomputed()) {
        float pidTime = (float) getPidTime();
        pidMotion.computationValues.pidTime = pidTime;

		PidMotionDefinition* motionDefinition = &(pidMotion.currentMotionDefinition);
        MotionInstruction* thetaInst = &(motionDefinition->inst[INSTRUCTION_THETA_INDEX]);
        MotionInstruction* alphaInst = &(motionDefinition->inst[INSTRUCTION_ALPHA_INDEX]);

		PidComputationValues* computationValues = &(pidMotion.computationValues);
        Motion* thetaMotion = &(computationValues->motion[INSTRUCTION_THETA_INDEX]);
        Motion* alphaMotion = &(computationValues->motion[INSTRUCTION_ALPHA_INDEX]);

        computeErrorsUsingCoders(&pidMotion);
        float thetaError = computationValues->thetaError;
        float alphaError = computationValues->alphaError;

		// Change PID type for final Approach
        if ((thetaError < ERROR_FOR_STRONG_PID) && (pidTime > thetaInst->t3 + TIME_PERIOD_AFTER_END_FOR_STRONG_PID)
                && (alphaError < ERROR_FOR_STRONG_PID) && (pidTime > alphaInst->t3 + TIME_PERIOD_AFTER_END_FOR_STRONG_PID)) {
            thetaInst->pidType = PID_TYPE_FINAL_APPROACH_INDEX;
            alphaInst->pidType = PID_TYPE_FINAL_APPROACH_INDEX;
        }

        // Computes the PID
		motionDefinition->computeU(&pidMotion);

        // 2 dependant Wheels (direction + angle)
        float leftMotorSpeed = (thetaMotion->u + alphaMotion->u) / 2.0f;
        float rightMotorSpeed = (thetaMotion->u - alphaMotion->u) / 2.0f;
        setMotorSpeeds(leftMotorSpeed, rightMotorSpeed);

        // If we maintain the position, we consider that we must maintain indefinitely the position
        if (thetaInst->motionType == MOTION_TYPE_MAINTAIN_POSITION) {
            return POSITION_TO_MAINTAIN;
        }

		MotionEndDetectionParameter* endDetectionParameter = getMotionEndDetectionParameter();
		MotionEndInfo* thetaEndMotion = &(computationValues->motionEnd[INSTRUCTION_THETA_INDEX]);
		MotionEndInfo* alphaEndMotion = &(computationValues->motionEnd[INSTRUCTION_ALPHA_INDEX]);
	
		thetaMotion->currentSpeed = thetaMotion->position - thetaMotion->oldPosition;
		alphaMotion->currentSpeed = alphaMotion->position - alphaMotion->oldPosition;

		updateEndMotionData(INSTRUCTION_THETA_INDEX, thetaEndMotion, endDetectionParameter, pidTime);
		updateEndMotionData(INSTRUCTION_ALPHA_INDEX, alphaEndMotion, endDetectionParameter, pidTime);

		BOOL isThetaEnd = isEndOfMotion(INSTRUCTION_THETA_INDEX, thetaEndMotion, endDetectionParameter);
		BOOL isAlphaEnd = isEndOfMotion(INSTRUCTION_ALPHA_INDEX, alphaEndMotion, endDetectionParameter);

		BOOL isThetaBlocked = isRobotBlocked(INSTRUCTION_THETA_INDEX, thetaEndMotion, endDetectionParameter);
		BOOL isAlphaBlocked = isRobotBlocked(INSTRUCTION_ALPHA_INDEX, alphaEndMotion, endDetectionParameter);

		if (isThetaEnd && isAlphaEnd) {
			if (isThetaBlocked || isAlphaBlocked) {
				return POSITION_BLOCKED_WHEELS;
            } else {
                return POSITION_REACHED;
            }
		}
    }
    return POSITION_IN_PROGRESS;
}
