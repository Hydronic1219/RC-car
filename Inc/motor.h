/*
 * motor.h
 *
 *  Created on: Jul 3, 2025
 *      Author: user2
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "main.h"

typedef enum {
    STOP = 0,
    FORWARD,
    BACKWARD
} DriveState;

typedef enum {
    None = 0,
    Left,
    Right
} TurnState;

extern DriveState currentState;
extern TurnState turn;

void motorInit();
void frontBehave();
void backBehave();
void front_Left();
void front_Right();
void back_Left();
void back_Right();
void stop();
void autofrontBehave();
void autobackBehave();
void autostop();
void auto_Right();
void auto_Left();

#endif /* INC_MOTOR_H_ */
