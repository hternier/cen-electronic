#ifndef GAME_TARGET_ACTION_LIST_H
#define GAME_TARGET_ACTION_LIST_H

#include "gameTargetAction.h"
#include "gameTargetActionItemList.h"

#include "../../common/io/outputStream.h"
#include "../../common/io/printWriter.h"

#include "../../navigation/LocationList.h"
#include "../../navigation/path.h"

/** The max limit of target action count. */
#define MAX_TARGET_ACTION 		3

/**
 * Tre struct defining a list of strategies.
 */
typedef struct GameTargetActionList {
    /** An array of pointer on actions. */
	GameTargetAction* actions[MAX_TARGET_ACTION];
    /** the size of the list. */
    unsigned char size;
} GameTargetActionList;

/**
 * Clear the target Action and the corresponding items.
 */
void clearTargetActionList(GameTargetActionList* targetActionList);

/**
 * Add an action to the target.
 * @param targetAction the target to add to the list
 */
void addTargetAction(GameTargetActionList* targetActionList,
					 GameTargetAction* targetAction,
					 Location* startLocation,
					 Location* endLocation,
					 int timeToAchieve,
					 PathDataFunction* pathDataFunction,
					 GameTargetActionItemList* actionListItem
);

/**
 * Get the target action at index.
 */
GameTargetAction* getGameTargetAction(GameTargetActionList* list, int index);

/**
 * Get the count of target action.
 */
int getGameTargetActionCount(GameTargetActionList* list);

/**
 * Print the game target action list, and for each the detail of target action
 */
void printGameTargetActionList(OutputStream* outputStream, GameTargetActionList* list);

#endif
