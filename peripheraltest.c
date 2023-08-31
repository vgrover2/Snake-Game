/*
 * interrupts.c
 *
 *  Created on: Dec 13, 2021
 *      Author: slsheahan
 */

#include "peripheraltest.h"

/*
 * Initializes the RGBs for conducting tests
 */
void initTesting(){
    ece353_MKII_RGB_IO_Init(false);
    ece353_rgb_init();
}

/*
    Tests the functionality of the accelerometer peripheral.
    When the peripheral is working properly:
    Left - red (Launchpad)  Right - red (MKII)
    Forward - green (L)      Backward - green (MKII)
    Up - blue (L)          Down - blue (MKII)
    Middle - off
*/
void testAccel(){
    accel_init();

    // Set up timer
    ece353_T32_1_Interrupt_Ms(100);

    // Turn on interrupts
    NVIC_EnableIRQ(ADC14_IRQn);
    __enable_irq();

      while(1){
        // X branch
        // Left
        if(ACCEL_X_DIR < 1500){
            ece353_MKII_RGB_LED(false, false, false);
            ece353_rgb(true, false, false);
        }
        // right
        else if (ACCEL_X_DIR > 2500){
            ece353_MKII_RGB_LED(true, false, false);
            ece353_rgb(false, false, false);
        }
        // Y branch
        // left
        else if(ACCEL_Y_DIR < 1500){
            ece353_MKII_RGB_LED(false, false, false);
            ece353_rgb(false, true, false);
        }
        // right
        else if (ACCEL_Y_DIR > 2500){
            ece353_MKII_RGB_LED(false, true, false);
            ece353_rgb(false, false, false);
        }
        // Z branch
        // left
        else if(ACCEL_Z_DIR < 1500){
            ece353_MKII_RGB_LED(false, false, false);
            ece353_rgb(false, false, true);
        }
        // right
        else if (ACCEL_Z_DIR > 3000){
            ece353_MKII_RGB_LED(false, false, true);
            ece353_rgb(false, false, false);
        }

        // center
        else {
            ece353_MKII_RGB_LED(false, false, false);
            ece353_rgb(false, false, false);
        }
    }
}

/*
    Tests the functionality of the light sensor peripheral
    When the peripheral is working properly:
    Dark - led (MKII) on
    Light - led (MKII) off
 */
void testLightSens(){
    i2c_init();
    opt3001_init();
    while(1){
        float lightLevel = opt3001_read_light();
        if(lightLevel > 3000){
            ece353_MKII_RGB_LED(false, false, false);
        } else {
            ece353_MKII_RGB_LED(true, true, true);
        }
    }
}


