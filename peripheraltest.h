/*
 * peripheraltest.h
 *
 *  Created on: Dec 13, 2021
 *      Author: slsheahan
 */

#ifndef __PERIPHERALTEST_H__
#define __PERIPHERALTEST_H__

#include "ece353.h"
#include "accelerometer.h"
#include "lightsensor.h"
#include "interrupts.h"

/*
 * Initializes the RGBs for conducting tests
 */
void initTesting();

/*
    Tests the functionality of the accelerometer peripheral.
    When the peripheral is working properly:
    Left - red (Launchpad)  Right - red (MKII)
    Forward - blue (L)      Backward - blue (MKII)
    Up - green (L)          Down - green (MKII)
    Middle - off
*/
void testAccel();

/*
    Tests the functionality of the light sensor peripheral
    When the peripheral is working properly:
    Dark - led (MKII) on
    Light - led (MKII) off
 */
void testLightSens();




#endif /* PERIPHERALTEST_H_ */
