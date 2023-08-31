/*
 * lightsensor.h
 *
 *  Created on: Dec 6, 2021
 *      Author: slsheahan
 */

#ifndef __LIGHTSENSOR_H__
#define __LIGHTSENSOR_H__

#include "msp.h"
#include "i2c.h"
#include <math.h>
#include <stdint.h>
#include <stdbool.h>


// J1.8, P4.6 - OPT3001 interrupt
// J1.9, P6.5 - I2C SCL
//J1.10, P6.4 - I2C SDA
// Pin is multiplexed with the I2C communication lines of the TMP006.
#define I2C_LIGHT_ADDR   0x44

#define I2C_LIGHT_RESULT        0x00
#define I2C_LIGHT_CONFIG        0x01
#define I2C_LIGHT_LOW_LIM       0x02
#define I2C_LIGHT_HI_LIM        0x03
#define I2C_MANUF_ID            0x7E
#define I2C_LIGHT_DEV_ID        0x7F

/* CONFIGURATION REGISTER SETTINGS */
#define OPT3001_ZEROS           0x0000
#define OPT3001_FULL_SCL_RNG    0xC000
#define OPT3001_POWER_UP_CONT   0x0600
#define OPT3001_LATCH_WINDOW    0x0010


/******************************************************************************
 * Initialize the OPT3001 light sensor on the MKII.  This function assumes
 * that the I2C interface has already been configured to operate at 100KHz.
 ******************************************************************************/
void opt3001_init(void);

/******************************************************************************
 * Returns the current light level in lux.
 ******************************************************************************/
float opt3001_read_light(void);

#endif /* PERIPHERAL_FILES_LIGHTSENSOR_H_ */
