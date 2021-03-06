#ifndef ROBOT_INFRARED_DETECTOR_H
#define ROBOT_INFRARED_DETECTOR_H

#include "../../common/commons.h"

/** 
 * Define the underlying function type attached to a group to evaluate if 
 * the infrared detect something.
 */
typedef BOOL InfraredDetectorFunction();

/**
 * Detection is based on 2 GPD12 on left / right, and a gp2y0a02yk on the center
 */
typedef struct InfraredDetectorGroup {
	/** The number of count which was detected. */
	int wasDetectedCount;
	/** A counter to avoid to notify. */
	BOOL doNotCheckBeforeCounter;
	/** A counter of interruption. */
	unsigned int interruptCounter;
	/** The function to evaluate it. */
	InfraredDetectorFunction* function;
}InfraredDetectorGroup;


/**
 * Init and a timer to search via infrared detector.
 */
void initRobotInfraredDetector();

/**
 * Returns if there is an obstacle forward.
 */
BOOL getRobotInfraredObstacleForward();

/**
 * Returns if there is an obstacle backward.
 */
BOOL getRobotInfraredObstacleBackward();

#endif
