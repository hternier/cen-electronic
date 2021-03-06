#include <i2c.h>

#include "i2cMasterSetup.h"

#include "../../../common/commons.h"
#include "../../../common/i2c/i2cCommon.h"

#include "../../../common/log/logger.h"
#include "../../../common/log/logLevel.h"
#include "../../../common/io/printWriter.h"
#include "../../../common/io/binaryPrintWriter.h"

static BOOL initialized = FALSE;

void i2cMasterInitialize(void) {
    // Avoid more than one initialization
    if (initialized) {
        appendString(getOutputStreamLogger(DEBUG), "I2C Master already initialized\n");
        return;
    }
    unsigned int i2cBrg, i2cCon;

    // Speed 100 Khz
    i2cBrg = 0x10B;
    // Configure I2C for 7 bit address mode
    i2cCon = (
            I2C_ON
            & I2C_IDLE_CON
            & I2C_CLK_HLD
            & I2C_IPMI_DIS
            & I2C_7BIT_ADD
            & I2C_SLW_DIS
            & I2C_SM_DIS
            & I2C_GCALL_DIS
            & I2C_STR_DIS
            & I2C_NACK
            & I2C_ACK_EN
            & I2C_RCV_EN
            & I2C_STOP_DIS
            & I2C_RESTART_DIS
            & I2C_START_DIS
            );

    OpenI2C(i2cCon, i2cBrg);
    WaitI2C();

    appendString(getOutputStreamLogger(DEBUG), "I2C Master CONF=");
    appendBinary16(getOutputStreamLogger(DEBUG), I2CCON, 4);
    appendCRLF(getOutputStreamLogger(DEBUG));

    initialized = TRUE;
}

void i2cMasterFinalize(void) {
    if (initialized) {
        initialized = FALSE;
        CloseI2C();
    }
}
