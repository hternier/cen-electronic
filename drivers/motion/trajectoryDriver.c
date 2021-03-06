#include "trajectoryDriver.h"

#include "../../common/commons.h"

#include "../../common/io/buffer.h"
#include "../../common/io/ioUtils.h"
#include "../../common/io/inputStream.h"
#include "../../common/io/outputStream.h"
#include "../../common/io/printWriter.h"
#include "../../common/io/reader.h"
#include "../../common/io/stream.h"

#include "../../common/log/logger.h"
#include "../../common/log/logHandler.h"

#include "../../device/motion/position/trajectoryDeviceInterface.h"

#include "../../drivers/driver.h"
#include "../../drivers/driverList.h"
#include "../../drivers/driverTransmitter.h"

#include "../../robot/robot.h"


// SET TRAJECTORY

BOOL trajectoryDriverSetAbsolutePosition(float x, float y, float deciDegree) {
    OutputStream* outputStream = getDriverRequestOutputStream();
    append(outputStream, COMMAND_SET_ABSOLUTE_POSITION);
    appendHex4(outputStream, x);
	appendSeparator(outputStream);
    appendHex4(outputStream, y);
	appendSeparator(outputStream);
    appendHex4(outputStream, deciDegree);

    BOOL result = transmitFromDriverRequestBuffer();

    return result;
}

// UPDATE TRAJECTORY DEVICE -> ROBOT POSITION
BOOL trajectoryDriverUpdateRobotPosition() {
    OutputStream* outputStream = getDriverRequestOutputStream();
    InputStream* inputStream = getDriverResponseInputStream();

    append(outputStream, COMMAND_GET_ABSOLUTE_POSITION);

    BOOL result = transmitFromDriverRequestBuffer();
    if (result) {
        int x = readHex4(inputStream);
		readHex(inputStream);
        int y = readHex4(inputStream);
		readHex(inputStream);
        int angle = readHex4(inputStream);

		updateRobotPosition(x, y, angle);
		return result;

    }
    return FALSE;
}
