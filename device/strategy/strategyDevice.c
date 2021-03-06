#include "strategyDevice.h"
#include "strategyDeviceInterface.h"

#include "../../common/cmd/commonCommand.h"

#include "../../common/io/printWriter.h"
#include "../../common/io/reader.h"
#include "../../common/io/stream.h"

#include "../../common/log/logger.h"
#include "../../common/log/logLevel.h"

#include "../../common/io/outputStream.h"
#include "../../common/io/printWriter.h"

#include "../../device/motion/simple/motionDeviceInterface.h"

#include "../../robot/strategy/gameStrategyHandler.h"
#include "../../robot/strategy/gameStrategyContext.h"

#include "../../robot/gameboard/gameboard.h"

#include "../../robot/2012/strategy2012.h"
#include "../../robot/2012/strategy2012Utils.h"
#include "../../robot/config/robotConfig.h"

// DEVICE INTERFACE

void initStrategy(void) {
    
}

void stopStrategy(void) {

}

BOOL isStrategyDeviceOk(void) {
    return TRUE;
}

void deviceStrategyHandleRawData(char header, InputStream* inputStream, OutputStream* outputStream) {
 	if (header == COMMAND_STRATEGY_SET_OPPONENT_ROBOT_POSITION) {

		// data
		int x = readHex4(inputStream);
		checkIsChar(inputStream, '-');
		int y = readHex4(inputStream);

		GameStrategyContext* context = getStrategyContext();

		context->opponentRobotPosition.x = x;
		if (isViolet()) {
			context->opponentRobotPosition.y = y;
		}
		else {
			// Opponent Robot position is relative to violet !
			context->opponentRobotPosition.y = GAMEBOARD_HEIGHT - y;
		}
		updatePathsAvailability();

        appendAck(outputStream);
        append(outputStream, COMMAND_STRATEGY_SET_OPPONENT_ROBOT_POSITION);
	}
	else if (header == COMMAND_STRATEGY_SET_CONFIG) {
		// data
		int c = readHex4(inputStream);
        appendAck(outputStream);
        append(outputStream, COMMAND_STRATEGY_SET_CONFIG);

		GameStrategyContext* context = getStrategyContext();
		
		// TODO : Provide non specific function
		int strategyIndex = (c & CONFIG_STRATEGY_MASK);
		context->strategyIndex = strategyIndex;
		appendStringAndDec(getOutputStreamLogger(INFO), "setStrategy:", strategyIndex);
		println(getOutputStreamLogger(INFO));

		initStrategy2012(strategyIndex);
		if (c & CONFIG_COLOR_BLUE_MASK) {
			setColor(COLOR_VIOLET);
		}
		else {
			setColor(COLOR_RED);
		}

	}
	// Print Gameboard
	else if (header == COMMAND_STRATEGY_PRINT_GAME_BOARD) {
		OutputStream* debugOutputStream = getOutputStreamLogger(ALWAYS);
        appendAck(outputStream);
        append(outputStream, COMMAND_STRATEGY_PRINT_GAME_BOARD);

		printStrategyAllDatas(debugOutputStream);
		printGameboard(debugOutputStream);
	}
	// next step
	else if (header == COMMAND_STRATEGY_NEXT_STEP) {
		GameStrategyContext* context = getStrategyContext();
		// response
        appendAck(outputStream);
        append(outputStream, COMMAND_STRATEGY_NEXT_STEP);
		// output arguments : we have only last information !
		appendHex2(outputStream, context->hasMoreNextSteps);

		// do the job synchronously to avoid problems of notification
		context->hasMoreNextSteps = nextStep();
	}
	else if (header == COMMAND_STRATEGY_SET_ROBOT_POSITION) {
		GameStrategyContext* context = getStrategyContext();
		// output before any notification !!
        appendAck(outputStream);
        append(outputStream, COMMAND_STRATEGY_SET_ROBOT_POSITION);
		
		// status
		unsigned int status = readHex2(inputStream);
		// separator
		checkIsChar(inputStream, '-');
		// x
		context->robotPosition.x = readHex4(inputStream);
		// separator
		checkIsChar(inputStream, '-');
		// y
		context->robotPosition.y = readHex4(inputStream);
		// separator
		checkIsChar(inputStream, '-');
		// angle in ddeg
		context->robotAngle = readHex4(inputStream);

		OutputStream* debugOutputStream = getOutputStreamLogger(INFO);
		appendStringAndDec(debugOutputStream, "\nStrategySetRobotPosition:status=", status);
		appendStringAndDec(debugOutputStream, ", x=", context->robotPosition.x);
		appendStringAndDec(debugOutputStream, ", y=", context->robotPosition.y);
		appendStringAndDec(debugOutputStream, ", angle=", context->robotAngle);
		println(debugOutputStream);
	
		// After Robot position update, if the status corresponds to collision, handle collision
		if (status == NOTIFY_MOTION_ARG_OBSTACLE) {
			handleCollision();
		}
	}
}

static DeviceDescriptor descriptor = {
    .deviceInit = &initStrategy,
    .deviceShutDown = &stopStrategy,
    .deviceIsOk = &isStrategyDeviceOk,
    .deviceHandleRawData = &deviceStrategyHandleRawData,
};

DeviceDescriptor* getStrategyDeviceDescriptor() {
    return &descriptor;
}
