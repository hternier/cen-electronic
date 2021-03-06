#include "gameTargetList.h"
#include "gameTarget.h"

#include "../../common/error/error.h"

#include "../../navigation/navigation.h"


/**
 * Target list.
 * Singleton
 */
static GameTargetList targets;

GameTargetList* getGameTargetList() {
	return &targets;
}

void clearGameTargets() {
	int i;
	int size = targets.size;
	for (i = 0; i < size; i++) {
		GameTarget* target = targets.targets[i];
		clearGameTarget(target);
	}	
	targets.size = 0;
	targets.targetHandledCount = 0;
}

void incTargetHandledCount() {
	targets.targetHandledCount++;
}

unsigned char getTargetHandledCount() {
	return targets.targetHandledCount;
}

void addGameTarget(GameTarget* target, char* targetName, int gain, Location* location) {
    unsigned char size = targets.size;
	if (size < MAX_TARGET) {
		target->name = targetName;
		target->status = TARGET_AVAILABLE;
		target->gain = gain;
		target->location = location;
	    targets.targets[size] = target;
	    targets.size++;
	}
	else {
		writeError(TOO_MUCH_TARGETS);
    }
}

GameTarget* getGameTarget(int index) {
    return targets.targets[index];
}

int getGameTargetCount() {
    return targets.size;
}

BOOL containsAvailableTarget() {
	int i;
	int size = targets.size;
	for (i = 0; i < size; i++) {
		GameTarget* target = targets.targets[i];
		if (target->status == TARGET_AVAILABLE) {
			return TRUE;
		}	
	}
	return FALSE;
}

// DEBUG

void printGameTargetList(OutputStream* outputStream) {
	int i;
	int size = targets.size;
	println(outputStream);	
	appendStringAndDec(outputStream, "targetHandledCount:", targets.targetHandledCount);
	println(outputStream);
	for (i = 0; i < size; i++) {
		GameTarget* target = targets.targets[i];
		printGameTarget(outputStream, target, TRUE);
	}
}
