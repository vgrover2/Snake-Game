/*
 * interrupts.h
 *
 *  Created on: Dec 6, 2021
 *      Author: slsheahan
 */

#ifndef __INTERRUPTS_H__
#define __INTERRUPTS_H__

#include <stdbool.h>
#include <stdint.h>
#include "ece353.h"
#include "msp.h"
#include "main.h"

extern volatile uint16_t ACCEL_X_DIR;
extern volatile uint16_t ACCEL_Y_DIR;
extern volatile uint16_t ACCEL_Z_DIR;
extern volatile uint16_t PS2_X_DIR;
extern volatile uint16_t PS2_Y_DIR;
extern volatile bool ALERT_S1;
extern volatile bool ALERT_S2;

// Initializes button 1 on the MKII
void init_s1_MKII(void);

// Initializes button 2 on the MKII
void init_s2_MKII(void);

// Handles Timer Interrupts
void T32_INT1_IRQHandler(void);

// Handles accelerometer ADC interrupts
void ADC14_IRQHandler(void);

// Sets a specified interrupt timer
void ece353_T32_1_Interrupt_Ms(uint32_t delay);

// Debouncing functions
bool debounce_s1(void);
bool debounce_s2(void);

#endif /* INTERRUPTS_H_ */
