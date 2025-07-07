/*
 * ultraSonic.h
 *
 *  Created on: Jul 4, 2025
 *      Author: user2
 */

#ifndef INC_ULTRASONIC_H_
#define INC_ULTRASONIC_H_

#include "main.h"
#include "delay_us.h"

//#define TRIG_PORT_R   GPIOB
//#define TRIG_PIN_R    GPIO_PIN_13
//#define TRIG_PORT_M   GPIOB
//#define TRIG_PIN_M    GPIO_PIN_14
#define TRIG_PORT_L   GPIOB
#define TRIG_PIN_L    GPIO_PIN_15

void HCSR04_TRIG_R(void);
void HCSR04_TRIG_M(void);
void HCSR04_TRIG_L(void);

#endif /* INC_ULTRASONIC_H_ */
