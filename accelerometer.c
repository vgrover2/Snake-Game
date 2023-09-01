/*
 * accelerometer.c
 *
 *  Created on: Dec 6, 2021
 *      Author: slsheahan and Vedant Grover
 */
#include "accelerometer.h"

void accel_init(void){
    // Configure the X direction as an analog input pin
    ACCEL_X_PORT->SEL0 |= ACCEL_X_PIN;
    ACCEL_X_PORT->SEL1 |= ACCEL_X_PIN;

    // Configure the Y direction as an analog input pin
    ACCEL_Y_PORT->SEL0 |= ACCEL_Y_PIN;
    ACCEL_Y_PORT->SEL1 |= ACCEL_Y_PIN;

    // Configure the Z direction as an analog input pin
    ACCEL_Z_PORT->SEL0 |= ACCEL_Z_PIN;
    ACCEL_Z_PORT->SEL1 |= ACCEL_Z_PIN;

    // Configure CTL0 to sample 16-times in pulsed sample mode.
    // NEW -- Indicate that this is a sequence-of-channels.
    ADC14->CTL0 = ADC14_CTL0_SHP | ADC14_CTL0_CONSEQ_1
                |ADC14_CTL0_SHT0_2 | ADC14_CTL0_MSC;
    ADC14->CTL0 |= ADC14_CTL0_BUSY;

    // Configure ADC to return 12-bit values
    ADC14->CTL1 |= ADC14_CTL1_RES_2;

    // Associate the X direction analog signal with MEM[0]
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_14;

    // Associate the Y direction analog signal with MEM[1]
    ADC14->MCTL[1] = ADC14_MCTLN_INCH_13;

    // Associate the Z direction analog signal with MEM[2]
    ADC14->MCTL[2] = ADC14_MCTLN_INCH_11;
    ADC14->MCTL[2] |= ADC14_MCTLN_EOS;


    ADC14->CTL0 &= ~ADC14_CTL0_BUSY;
    // Enable interrupts in the ADC AFTER a value is written into MEM[1].
    //
    // NEW: This is not the same as what is demonstrated in the example
    // coding video.
    ADC14->IER0 |= ADC14_IER0_IE2;

    // Enable ADC Interrupt in the NVIC
    NVIC_EnableIRQ(ADC14_IRQn);

    // Turn ADC ON
    ADC14->CTL0 |= ADC14_CTL0_ON;
}


