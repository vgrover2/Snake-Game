/*
 * lightsensor.c
 *
 *  Created on: Dec 6, 2021
 *      Author: Vedant Grover
 */

#include "lightsensor.h"
#include "i2c.h"

/******************************************************************************
 * Initialize the OPT3001 temperature sensor on the MKII.  This function assumes
 * that the I2C interface has already been configured to operate at 100KHz.
 ******************************************************************************/
void opt3001_init(void)
{
    int i;

    // Reset the device using the CONFIG register
    i2c_write_16(I2C_LIGHT_ADDR, I2C_LIGHT_CONFIG, OPT3001_ZEROS);

    // delay
    for(i = 0; i < 50000; i++){};

    // Program the CONFIG register to POWER_UP and bein CR_2 mode
    i2c_write_16(I2C_LIGHT_ADDR, I2C_LIGHT_CONFIG, OPT3001_FULL_SCL_RNG |
                                                 OPT3001_POWER_UP_CONT |
                                                 OPT3001_LATCH_WINDOW);
}

/******************************************************************************
 * Returns the current light in lux
 ******************************************************************************/
float opt3001_read_light(void)
{
    uint16_t result;
    uint16_t LSB_Size;
    uint16_t lux;

    // Read the ambient light
    result = i2c_read_16(I2C_LIGHT_ADDR, I2C_LIGHT_RESULT);

    // Return the data in lux
    // You will need to modify the line below to return this value
    LSB_Size = 0.01 * pow(2, (result & 0xF000));
    lux = LSB_Size * (result & 0x0FFF);

    return lux;
}

