#include "codersDeviceInterface.h"

#include "../../../device/device.h"
#include "../../../device/deviceInterface.h"

const char* getCodersDeviceName(void) {
    return "Hctl Coders";
}

int deviceCodersGetInterface(char header, int mode, BOOL fillDeviceArgumentList) {
    if (header == COMMAND_CLEAR_CODERS) {
		// Same INPUT / OUTPUT
		if (fillDeviceArgumentList) {
			setFunctionNoArgument("clrCoders");
		}
        return 0;
    } else if (header == COMMAND_GET_WHEEL_POSITION) {
        if (mode == DEVICE_MODE_INPUT) {
            if (fillDeviceArgumentList) {
				setFunctionNoArgument("codersVal");
            }
            return 0;
        } else if (mode == DEVICE_MODE_OUTPUT) {
            if (fillDeviceArgumentList) {
				setFunction("codersVal", 3);
				setArgument(0, DEVICE_ARG_UNSIGNED_HEX_8, "left");
                setArgumentSeparator(1);
				setArgument(2, DEVICE_ARG_UNSIGNED_HEX_8, "right");
            }
            return 17;
        }
    } else if (header == COMMAND_DEBUG_GET_WHEEL_POSITION) {
		// Same INPUT / OUTPUT
		if (fillDeviceArgumentList) {
			setFunctionNoArgument("dbgCodersVal");
		}
		return 0;
    }
    return DEVICE_HEADER_NOT_HANDLED;
}

static DeviceInterface deviceInterface = {
    .deviceGetName = &getCodersDeviceName,
    .deviceGetInterface = &deviceCodersGetInterface
};

DeviceInterface* getCodersDeviceInterface() {
    return &deviceInterface;
}
