#ifndef ROBOT_CONFIG_DEVICE_H
#define ROBOT_CONFIG_DEVICE_H

#include "../../device/device.h"

/**
* Returns a descriptor on the config.
* @return the descriptor of the device.
*/
DeviceDescriptor* getRobotConfigDeviceDescriptor();

/**
* Refresh the config in memory
*/
void refreshConfig( void );

/**
* Returns if the config is present or not.
*/
// char isConfigPresent( void );

void appendStringConfig(OutputStream* outputStream);

/**
* Reads the raw config Value.
* You must call refreshConfig for the first time
*/
unsigned int  getConfigValue( void );

/**
* Returns if the specified configMask is set or not
*/
int isConfigSet(unsigned int configMask);

/**
* Return for a configIndex given in parameter the string representation of the configuration
* @param configIndex the index for the config
*/
char* getConfigBitString(unsigned char configIndex);

/**
*/
unsigned char isConfigBalise();

/**
* Returns the strategy which must be used by the robot.
*/
unsigned char getStrategy();

#endif
