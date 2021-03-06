#include <math.h>

#include "cenMath.h"

#include "../../common/commons.h"

BOOL floatEquals(float value1, float value2) {
    // FIXME : Take care about precision
    return value1 == value2;
}

BOOL floatEqualsZero(float value1) {
    return (value1 >= -0.00001f && value1 <= 0.00001f);
}

long absLong(signed long value) {
    if (value < 0L) {
        return -value;
    } else {
        return value;
    }
}

signed long limit(signed long value, signed long maxValue) {
    if (value > maxValue) {
        return maxValue;
    } else if (value < -maxValue) {
        return -maxValue;
    } else {
        return value;
    }
}

signed int compareFloat(float a, float b) {
    if (a > b) {
        return 1;
    } else if (a < b) {
        return -1;
    } else {
        return 0;
    }
}

signed int getSign(float a) {
    if (a < 0) {
        return -1;
    } else if (a > 0) {
        return 1;
    } else {
        return 0;
    }
}

int minInt(int value1, int value2) {
    if (value1 < value2) {
        return value1;
    }
    return value2;
}

int maxInt(int value1, int value2) {
    if (value1 > value2) {
        return value1;
    }
    return value2;
}

float mod2PI(float value) {
	if (value < - PI) {
		return (value + _2_PI);
	} else if (value > PI) {
		return (value - _2_PI);
	}
	return value;
}

int stringChecksum(char* string) {
	int result = 0;
	while( *string ) {
		result += *string++;
	}
	return result;
}
