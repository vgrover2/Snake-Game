/*
 * interrupts.c
 *
 *  Created on: Dec 6, 2021
 *      Author: slsheahan
 */

#include "interrupts.h"

// Add a global variable that holds the most recent value of the X direction
volatile uint16_t ACCEL_X_DIR;
// Add a global variable that holds the most recent value of the Y direction
volatile uint16_t ACCEL_Y_DIR;
// Add a global variable that holds the most recent value of the Z direction
volatile uint16_t ACCEL_Z_DIR;

// Add a global variable that holds the most recent value of the X direction
volatile uint16_t PS2_X_DIR;
// Add a global variable that holds the most recent value of the Y direction
volatile uint16_t PS2_Y_DIR;

// Alerts if S1 or S2 are pressed on the MKII, respectively
volatile bool ALERT_S1;
volatile bool ALERT_S2;

/*
 * \brief   Generates an interrupt every delay milliseconds
 *
 * \param   delay - The amount of time to interrupt (in mS)
 *
 * \return  none
 */
void ece353_T32_1_Interrupt_Ms(uint32_t delay){
    uint32_t ticks = ((SystemCoreClock * delay) / 1000) - 1;

    TIMER32_1->CONTROL = 0;

    TIMER32_1->LOAD = ticks;

    __enable_irq();
    NVIC_EnableIRQ(T32_INT1_IRQn);
    NVIC_SetPriority(T32_INT1_IRQn, 1);

    TIMER32_1->CONTROL = TIMER32_CONTROL_ENABLE |
                         TIMER32_CONTROL_MODE |
                         TIMER32_CONTROL_SIZE |
                         TIMER32_CONTROL_IE;

}

//*****************************************************************************
// Function used to initialize the IO pin
// connected to the S1 button on the MKII
//*****************************************************************************
void init_s1_MKII(void)
{
    // Configure the pin connected to S1 as an input
    P5->DIR &= ~BIT1;
    P5->REN |= BIT1;
    P5->OUT |= BIT1;

    // Set the interrupt event to be a High to Low transition
    // IES Register (Interrupt Edge Select)
    P5->IES |= BIT1;

    // Enable interrupts in the peripheral
    P5->IE |= BIT1;

    // Enable the IO Port interrupts in the NVIC
    NVIC_EnableIRQ(PORT5_IRQn);

    // Set the priority of the IO Port interrupt to 0
    NVIC_SetPriority(PORT5_IRQn, 2);

}

//*****************************************************************************
// Function used to initialize the IO pin
// connected to the S2 button on the MKII
//*****************************************************************************
void init_s2_MKII(void)
{
    // Configure the pin connected to S3 as an input
    P3->DIR &= ~BIT5;
    P3->REN |= BIT5;
    P3->OUT |= BIT5;

    // Set the interrupt event to be a High to Low transition
    // IES Register (Interrupt Edge Select)
    P3->IES |= BIT5;

    // Enable interrupts in the peripheral
    P3->IE |= BIT5;

    // Enable the IO Port interrupts in the NVIC
    NVIC_EnableIRQ(PORT3_IRQn);

    // Set the priority of the IO Port interrupt to 2
    NVIC_SetPriority(PORT3_IRQn, 2);

}

// Timer interrupt to start ADC conversion
void T32_INT1_IRQHandler(void)
{
    BaseType_t xHigherPriorityTaskWoken;
    ALERT_S1 = debounce_s1();
    ALERT_S2 = debounce_s2();

    // Start the ADC conversion
    ADC14->CTL0 |= ADC14_CTL0_SC | ADC14_CTL0_ENC;

    // Check s1 - start game status
    if(ALERT_S1)
    {
        /* Send a notification directly to the task to which interrupt processing is
        being deferred. */
        vTaskNotifyGiveFromISR(
                TaskHandle_startGame,
                &xHigherPriorityTaskWoken
        );

        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
    // Check s2 - toggle motion
    else if (ALERT_S2){
        /* Send a notification directly to the task to which interrupt processing is
        being deferred. */
        vTaskNotifyGiveFromISR(
                TaskHandle_toggleMotion,
                &xHigherPriorityTaskWoken
        );

        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
    // Check light level
    else if ((opt3001_read_light() < 3000 && lightMode == true) ||
              opt3001_read_light() > 3000 && lightMode == false){
        /* Send a notification directly to the task to which interrupt processing is
        being deferred. */
        vTaskNotifyGiveFromISR(
                TaskHandle_toggleDarkMode,
                &xHigherPriorityTaskWoken
        );
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
    // Check game over status
    if(gameIsOver && !menuScreen){
        /* Send a notification directly to the task to which interrupt processing is
        being deferred. */
        vTaskNotifyGiveFromISR(
                TaskHandle_gameOver,
                &xHigherPriorityTaskWoken
        );
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
    // Check if in menu
    if(menuScreen){
        /* Send a notification directly to the task to which interrupt processing is
        being deferred. */
        vTaskNotifyGiveFromISR(
                TaskHandle_menuScreen,
                &xHigherPriorityTaskWoken
        );
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }

    // Clear the timer interrupt
    TIMER32_1->INTCLR = BIT0;
}

// Get ADC values
void ADC14_IRQHandler(void)
{
    // Motion Controls
    // Read the X value
    ACCEL_X_DIR = ADC14->MEM[0];
    // Read the Y value
    ACCEL_Y_DIR = ADC14->MEM[1];
    // Read the Z value
    ACCEL_Z_DIR = ADC14->MEM[2];

    // Joystick Controls
    // Read the X value
    PS2_X_DIR = ADC14->MEM[3];
    // Read the Y value
    PS2_Y_DIR = ADC14->MEM[4];

}

/******************************************************************************
 * De-bounce switch S2.
 *****************************************************************************/
__inline bool debounce_s2(void)
{
    static uint8_t debounce_state = 0x00;

    // Shift the de-bounce variable to the right
    debounce_state = debounce_state << 1;

    // If S1 is being pressed, set the LSBit of debounce_state to a 1;
    if(ece353_MKII_S2())
    {
        debounce_state |= 0x01;
    }

    // If the de-bounce variable is equal to 0x7F, change the color of the tri-color LED.
    if(debounce_state == 0x7F)
    {
        return true;
    }
    else
    {
        return false;
    }

}

/******************************************************************************
 * De-bounce switch S1.
 *****************************************************************************/
bool debounce_s1(void)
{
    static uint8_t debounce_state = 0x00;

    // Shift the de-bounce variable to the right
    debounce_state = debounce_state << 1;

    // If S1 is being pressed, set the LSBit of debounce_state to a 1;
    if(ece353_MKII_S1())
    {
        debounce_state |= 0x01;
    }

    // If the de-bounce variable is equal to 0x7F, change the color of the tri-color LED.
    if(debounce_state == 0x7F)
    {
        return true;
    }
    else
    {
        return false;
    }

}

//*****************************************************************************
// Interrupt Handler for the IO Port connected to S2
//
// Be sure to change the name of the handler to the correct handler
// name
//*****************************************************************************
void PORT3_IRQHandler()
{
    // Clear the interrupt.  Be sure to read the section of the
    // MSP432 TRM related to IO Interrupts to determine how to clear
    // an interrupt.
    P3->IFG &= !BIT5;

}

//*****************************************************************************
// Interrupt Handler for the IO Port connected to S1
//
// Be sure to change the name of the handler to the correct handler
// name
//*****************************************************************************
void PORT5_IRQHandler()
{
    // Clear the interrupt.  Be sure to read the section of the
    // MSP432 TRM related to IO Interrupts to determine how to clear
    // an interrupt.
    P5->IFG &= !BIT1;

}
