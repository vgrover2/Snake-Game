#include "ece353.h"
//****************************************************
//****************************************************
// 02-Ex-Code-Organization
//****************************************************
//****************************************************

/* ***************************************************
 * Initialize hardware resources used to control LED1
 *****************************************************/
void ece353_led1_init(void)
{
    P1->DIR |= BIT0;
    P1->OUT &= ~BIT0;
}

/*****************************************************
 * Initialize hardware resources used to control Button1
 *****************************************************/
void ece353_button1_init(void)
{
    P1->DIR &= ~BIT1;
    P1->REN |= BIT1;
    P1->OUT |= BIT1;
}

/*****************************************************
 * Turn LED1 ON/Off.
 *
 * Parameters
 *  on: if true,  turn LED on
 *      if false, turn LED off
 *****************************************************/
void ece353_led1(bool on)
{
    if(on){
        P1->OUT |= BIT0;
    } else {
        P1->OUT &= ~BIT0;
    }
}

/*****************************************************
 * Returns if Button1 is currently pressed.
 *
 * Parameters
 *
 * Returns
 *      true    :   Button1 is pressed
 *      false   :   Button1 is NOT pressed
 *****************************************************/
bool ece353_button1(void)
{
    if((P1->IN & BIT1) == 0){
        return true;
    } else {
        return false;
    }
}



//****************************************************
//****************************************************
// 02-ICE-Code-Organization
//****************************************************
//****************************************************

/*****************************************************
 * Initialize hardware resources used to control RGBLED
 *****************************************************/
void ece353_rgb_init(void)
{
    // red
    P2->DIR |= BIT0;
    P2->OUT &= ~BIT0;

    // green
    P2->DIR |= BIT1;
    P2->OUT &= ~BIT1;

    // blue
    P2->DIR |= BIT2;
    P2->OUT &= ~BIT2;
}

/*****************************************************
 * Turn RGBLED ON/Off.
 *
 * Parameters
 *  red_on      :   if true,  turn RGBLED.RED on
 *                  if false, turn RGBLED.RED off
 *  green_on    :   if true,  turn RGBLED.GREEN on
 *                  if false, turn RGBLED.GREEN off
 *  blue_on     :   if true,  turn RGBLED.BLUE on
 *                  if false, turn RGBLED.BLUE off
 *****************************************************/
void ece353_rgb(bool red_on, bool green_on, bool blue_on)
{

    // red LED
    if(red_on){
        P2->OUT |= BIT0;
    } else {
        P2->OUT &= ~BIT0;
    }

    // green LED
    if(green_on){
        P2->OUT |= BIT1;
    } else {
        P2->OUT &= ~BIT1;
    }

    // blue LED
    if(blue_on){
        P2->OUT |= BIT2;
    } else {
        P2->OUT &= ~BIT2;
    }


}

/*****************************************************
 * Initialize hardware resources used to control Button2
 *****************************************************/
void ece353_button2_init(void)
{
    P1->DIR &= ~BIT4;
    P1->REN |= BIT4;
    P1->OUT |= BIT4;
}

/*****************************************************
 * Returns if Button2 is currently pressed.
 *
 * Parameters
 *
 * Returns
 *      true    :   Button2 is pressed
 *      false   :   Button2 is NOT pressed
 *****************************************************/
bool ece353_button2(void)
{
    if((P1->IN & BIT4) == 0){
        return true;
    } else {
        return false;
    }
}

/*
 * Busy waits for 100 mS and then returns.
 */
void ece353_T32_1_wait_100mS(void){
    // 18750 ticks = 100 mS at 3 MHz with a prescaled clock
    uint32_t ticks = 18750;

    // Reset timer
    TIMER32_1->CONTROL = 0;

    // 16 bit timer
    TIMER32_1->CONTROL &= ~BIT1;

    // Oneshot
    TIMER32_1->CONTROL |= BIT0;

    // Prescale clock
    TIMER32_1->CONTROL &= ~BIT3;
    TIMER32_1->CONTROL |= BIT2;

    // Load Timer
    TIMER32_1->LOAD = ticks;

    // Start timer
    TIMER32_1->CONTROL |= TIMER32_CONTROL_ENABLE;

    // End condition
    while(TIMER32_1->VALUE != 0){
    }
}

/*
 * Debounces button1 using Timer32_1
 */
void ece353_button1_wait_for_press(void){
    while(1){
        int count = 0;

        while (ece353_button1()){
            ece353_T32_1_wait_100mS();
            count++;

            // Turn on light after 5 seconds
            if (count >= 50){
                return;
            }
        }
    }
}

//*****************************************************************************
//*****************************************************************************
// ICE 04 - PWMing MKII tri-color LED.
//*****************************************************************************
//*****************************************************************************

/*****************************************************
 * Initialize the 3 GPIO pins that control the RGB
 * LED on the MKII.
 *
 * Parameters
 *      None
 * Returns
 *      None
 *****************************************************/
void ece353_MKII_RGB_IO_Init(bool en_primary_function)
{

    // Complete the comments below to identify which pins
    // control which LEDs.
    //
    // Replace a and c with the correct port number
    // Replace b and d with the correct pin numbers.
    // RED      : P2.6
    // GREEN    : P2.4
    // BLUE     : P5.6

    // ADD CODE that configures the RED, GREEN, and
    // BLUE LEDs to be outputs
    P2->DIR |= BIT6;
    P2->OUT &= ~BIT6;

    P2->DIR |= BIT4;
    P2->OUT &= ~BIT4;

    P5->DIR |= BIT6;
    P2->OUT &= ~BIT6;

    // ADD CODE that selects the Primary module function
    // for all 3 pins
    if(en_primary_function)
    {
        P2->SEL1 &= ~BIT6;
        P2->SEL0 |= BIT6;

        P2->SEL1 &= ~BIT4;
        P2->SEL0 |= BIT4;

        P5->SEL1 &= ~BIT6;
        P5->SEL0 |= BIT6;
    }

}

/*****************************************************
 * Sets the PWM levels for the MKII RGBLED
 *
 * Parameters
 *      ticks_period    :   Period of PWM Pulse
 *      red_num         :   RED RGB Number
 *      green_num       :   GREEN RGB Number
 *      blue_num        :   BLUE RGB Number
 * Returns
 *      None
 *****************************************************/
void ece353_MKII_RGB_PWM(
        uint16_t ticks_period,
        uint16_t red_num,
        uint16_t green_num,
        uint16_t blue_num
)
{
    // This code converts the HTML color codes into a 
    // number of clock ticks used to generate the duty cyle of
    // the PWM signal.
    uint16_t ticks_red_on = (red_num * ticks_period)/255;
    uint16_t ticks_green_on = (green_num * ticks_period)/255;
    uint16_t ticks_blue_on = (blue_num * ticks_period)/255;

    // Initialize the IO pins used to control the
    // tri-color LED.
    ece353_MKII_RGB_IO_Init(true);

    // Complete the comments below that identify which
    // TimerA outputs will control the IO pins.
    //
    // Replace w and y with the correct TimerA number
    // Replace x and z with the correct CCR number.
    // RED      <--> TA0.3
    // GREEN    <--> TA0.1
    // BLUE     <--> TA2.1

    // ADD CODE BELOW
    //
    // Turn off the timer peripherals
    TIMER_A0->CTL = 0;
    TIMER_A2->CTL = 0;

    // Set the period for both TimerA peripherals.
    TIMER_A0->CCR[0] = ticks_period - 1;
    TIMER_A2->CCR[0] = ticks_period - 1;

    // Configure RED PWM duty cycle
    TIMER_A0->CCR[3] = ticks_red_on;

    // Configure GREEN PWM duty cycle
    TIMER_A0->CCR[1] = ticks_green_on;

    // Configure BLUE PWM duty cycle
    TIMER_A2->CCR[1] = ticks_blue_on;

    // Set the OUT MODE to be mode 7 for each
    // PWM output
    TIMER_A0->CCTL[3] = TIMER_A_CCTLN_OUTMOD_7;
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;
    TIMER_A2->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;

    // Turn the first TimerA peripheral.  Use SMCLK as the clock source
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK;
    TIMER_A0->CTL &= ~TIMER_A_CTL_MC_MASK;
    TIMER_A0->CTL |=  TIMER_A_CTL_MC__UP | TIMER_A_CTL_CLR;

    // Turn the second TimerA peripheral.  Use SMCLK as the clock source
    TIMER_A2->CTL = TIMER_A_CTL_SSEL__SMCLK;
    TIMER_A2->CTL &= ~TIMER_A_CTL_MC_MASK;
    TIMER_A2->CTL |=  TIMER_A_CTL_MC__UP | TIMER_A_CTL_CLR;
}

//*****************************************************************************
//*****************************************************************************
// ICE 06 - ADC14
//*****************************************************************************

/******************************************************************************
 * Configure the IO pins for BOTH the X and Y directions of the analog
 * joystick.  The X direction should be configured to place the results in
 * MEM[0].  The Y direction should be configured to place the results in MEM[1].
 *
 * After BOTH analog signals have finished being converted, a SINGLE interrupt
 * should be generated.
 *
 * Parameters
 *      None   
 * Returns
 *      None
 ******************************************************************************/
void ece353_ADC14_PS2_XY(void)
{
    // Configure the X direction as an analog input pin.
    //P6.0 x, P4.1 sel, P4.4 y
    P6->SEL0 |= BIT0;
    P6->SEL1 |= BIT0;

    // Configure the Y direction as an analog input pin.
    P4->SEL0 |= BIT4;
    P4->SEL1 |= BIT4;

    // Configure CTL0 to sample 16-times in pulsed sample mode.
    // NEW -- Indicate that this is a sequence-of-channels.
    ADC14->CTL0 = ADC14_CTL0_SHP | ADC14_CTL0_CONSEQ_1
                |ADC14_CTL0_SHT0_2 | ADC14_CTL0_MSC;

    ADC14->CTL0 |= ADC14_CTL0_BUSY;
    // Configure ADC to return 12-bit values
    ADC14->CTL1 = ADC14_CTL1_RES_2;

    // Associate the X direction analog signal with MEM[0]
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_15;

    // Associate the Y direction analog signal with MEM[1]
    // NEW -- Make sure to indicate this is the end of a sequence.
    ADC14->MCTL[1] = ADC14_MCTLN_INCH_9;
    ADC14->MCTL[1] |= ADC14_MCTLN_EOS;

    ADC14->CTL0 &= ~ADC14_CTL0_BUSY;
    // Enable interrupts in the ADC AFTER a value is written into MEM[1].
	//
	// NEW: This is not the same as what is demonstrated in the example
	// coding video.
    ADC14->IER0 |= ADC14_IER0_IE1;

    // Enable ADC Interrupt in the NVIC
    NVIC_EnableIRQ(ADC14_IRQn);

    // Turn ADC ON
    ADC14->CTL0 |= ADC14_CTL0_ON;
}

/*****************************************************
 * Turn RGB on the MKII LED ON/Off.
 *
 * Parameters
 *  red:    if true,  turn RED LED on
 *          if false, turn RED LED off
 *  green:  if true,  turn GREEN LED on
 *          if false, turn GREEN LED off
 *  blue:   if true,  turn BLUE LED on
 *          if false, turn BLUE LED off
 *****************************************************/
void ece353_MKII_RGB_LED(bool red, bool green, bool blue)
{
    if(red){
    P2->OUT |= BIT6;
    } else {
	P2->OUT &= ~BIT6;
    }

    if(green){
	P2->OUT |= BIT4;
    } else {
	P2->OUT &= ~BIT4;
    }

    if(blue){
 	P5->OUT |= BIT6;
    } else {
	P5->OUT &= ~BIT6;
    }
}

/*
 * Initialize RGB LED
 */
void ece353_RBG_LED_Init(){
    P2->DIR |= BIT2 | BIT1 | BIT0;
    P2->OUT = 0;
}

///*
// * This function was not found, unsure how to implement at the moment
// */
//void ece353_T32_1_Interrupt_Ms(uint32_t delay){
//    uint32_t ticks = ((SystemCoreClock * delay) / 1000) - 1;
//
//    TIMER32_1->CONTROL = 0;
//
//    TIMER32_1->LOAD = ticks;
//
//    __enable_irq();
//    NVIC_EnableIRQ(T32_INT1_IRQn);
//    NVIC_SetPriority(T32_INT1_IRQn, 0);
//
//    TIMER32_1->CONTROL = TIMER32_CONTROL_ENABLE |
//                         TIMER32_CONTROL_MODE |
//                         TIMER32_CONTROL_SIZE |
//                         TIMER32_CONTROL_IE;
//
//}


//*****************************************************************************
//*****************************************************************************
// ICE 07 - ADC14 Comparator
//*****************************************************************************
#define VOLT_0P85 1056      // 0.85 /(3.3/4096)
#define VOLT_2P50 3103      // 2.50 /(3.3/4096)

/******************************************************************************
 * Configure the IO pins for the X direction of the analog
 * joystick.  The X direction should be configured to place the results in
 * MEM[0].
 *
 * The ADC14 should be configured to generate interrupts using the Window
 * comparator.  The HI0 threshold should be set to 2.5V.  The LO0 threshold
 * should be set to 0.85V.
 *
 * Parameters
 *      None
 * Returns
 *      None
 ******************************************************************************/
void ece353_ADC14_PS2_XY_COMP(void)
{
    // Configure the X direction as an analog input pin.
    P6->SEL0 |= BIT0;
    P6->SEL1 |= BIT0;

    // Configure CTL0 to sample 16-times in pulsed sample mode.
    // Indicate that this is a sequence of samples.
    ADC14->CTL0 = ADC14_CTL0_SHP | ADC14_CTL0_CONSEQ_1
                | ADC14_CTL0_SHT0_2 | ADC14_CTL0_MSC;

    ADC14->CTL0 |= ADC14_CTL0_BUSY;
    // Configure CTL1 to return 12-bit values
    ADC14->CTL1 = ADC14_CTL1_RES_2;

    // Set up the HI0 Window
    ADC14->HI0 = VOLT_2P50;

    // Set up the LO0 Window
    ADC14->LO0 = VOLT_0P85;

    // Associate the X direction analog signal with MEM[0], indicate the end of sequence,
    // turn on the window comparator
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_15;
    ADC14->CTL0 &= ~ADC14_CTL0_BUSY;
    ADC14->MCTL[0] |= ADC14_MCTLN_WINC;


    // Enable interrupts when either the HI or LO thresholds of the window
    // comparator has been exceeded.  Disable all other interrupts
    ADC14->IER1 = 0;
    ADC14->IER1 |= ADC14_IER1_HIIE;
    ADC14->IER1 |= ADC14_IER1_LOIE;

    // Enable ADC Interrupt
    NVIC_EnableIRQ(ADC14_IRQn);

    // Turn ADC ON
    ADC14->CTL0 |= ADC14_CTL0_ON;
}

bool ece353_MKII_S1(){
    return false;
}

bool ece353_MKII_S2(){
    return false;
}
