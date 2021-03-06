#ifndef SERVO_PWM_H
#define SERVO_PWM_H

#include "pwmPic.h"

/**
* Defines the structure to manages Servos.
*/
typedef struct Servo {
	/** The speed to reach the final position. */
	unsigned int speed;
	/** The current position. */
	unsigned int currentPosition;
	/** The target position. */
	unsigned int targetPosition;
} Servo;

/**
* Tre struct defining a list of servo.
*/
typedef struct ServoList {
	Servo servos[PWM_COUNT];
} ServoList;

/**
* Middle position for a servo
*/
// FOR MD22 RC MODE
#define PWM_SERVO_MIDDLE_POSITION 1480

/**
* Left position (arround 45� / middle position).
*/
#define PWM_SERVO_LEFT_POSITION 1000

/**
* Right position (arround 45� / middle position).
*/
#define PWM_SERVO_RIGHT_POSITION 2000

#define PWM_SERVO_SPEED_MIN 1

#define PWM_SERVO_SPEED_MAX 255

// --- SERVO MODE ---

/**
* initPwm for a 50 Hz operation with a position
* Objet : Initialise le cycle de OC1 et OC2 � 20ms
* Utilisation du TIMER2
* @param posInt : Duty cycle de d�part de 0 � 20000
*/
void initPwmForServo (int dutyms);

/**
* do a pwm on a pwmIndex between 1 and 4
* @param dutyms duration of pwm to 1 typical value between
* PWM_SERVO_LEFT_POSITION and PWM_SERVO_RIGHT_POSITION 
*/
void pwmServo(int pwmIndex, unsigned int speed, int dutyms);

/**
* do a pwm on all pwm
* @param dutyms duration of pwm to 1 typical value between
* PWM_SERVO_LEFT_POSITION and PWM_SERVO_RIGHT_POSITION 
*/
void pwmServoAll(unsigned int speed, int dutyms);

/**
* Test all PWM.
*/
void testAllPwmServos();

#endif
