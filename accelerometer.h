/*
 * accelerometer.h
 *
 *  Created on: Dec 6, 2021
 *      Author: slsheahan
 */

#ifndef __ACCELEROMETER_H__
#define __ACCELEROMETER_H__

#include "msp.h"
#include "ece353.h"

// Accelerometer X - J3.23, P6.1
#define ACCEL_X_PORT    P6
#define ACCEL_X_PIN     BIT1

// Accelerometer Y - J3.24, P4.0
#define ACCEL_Y_PORT    P4
#define ACCEL_Y_PIN     BIT0

// Accelerometer Z - J3.25, P4.2
#define ACCEL_Z_PORT    P4
#define ACCEL_Z_PIN     BIT0


/*******************************************************************************
* Function Name: accel_init
********************************************************************************
* Summary: Initializes the accelerometer
* Returns:
*  Nothing
*******************************************************************************/
void accel_init();


#endif /* ACCELEROMETER_H_ */
