/*
 * task_mkII_s1.c
 *
 *  Created on: Oct 14, 2020
 *      Author: Joe Krachey
 */

#include "task_mkII_s1.h"

TaskHandle_t Task_mkII_s1_Handle = NULL;


/******************************************************************************
 * De-bounce switch S1.
 *****************************************************************************/
bool debounce_s1(void)
{
    static uint8_t debounce_state = 0x00;

    // Shift the de-bounce variable to the left
    debounce_state = debounce_state << 1;

    // If S1 is being pressed, set the LSBit of debounce_state to a 1;
    if(ece353_staff_MKII_S1())
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
 * De-bounce switch S1.  If is has been pressed, change the tri-Color LED on
 * the MKIIEDU . Everytime S1 is pressed, the color should change in
 * the following order:
 *      RED->GREEN->BLUE->RED->GREEN....
 *****************************************************************************/
void task_mkII_s1(void *pvParameters)
{
    BaseType_t status;
    LED_MSG_t msg;
    uint8_t led_color = 0;

    while(1)
    {

        if(debounce_s1())
        {
            led_color = (led_color + 1) % 3;

            /* ADD CODE
             * Take Sem_UART
             *
             */
            status = xSemaphoreTake(Sem_UART, portMAX_DELAY);

            printf("Button S1 has been pressed\n\r");

            /* ADD CODE
             * Give Sem_UART
             */
            xSemaphoreGive(Sem_UART);

            /* ADD CODE
             * Use the variable msg to :
             *      Set the location of the LED as the MKIIEDU.
             *
             *      The color of the led is determined by the value
             *      in led_color.
             *
             */
            msg.led_location = MKIIEDU;
            switch(led_color)
            {
            case 0:
            {
                msg.color = LED_RED;
                break;
            }
            case 1:
            {
                msg.color = LED_GREEN;
                break;
            }
            case 2:
            {
                msg.color = LED_BLUE;
                break;
            }


            }

            /* ADD CODE
             * Send msg to Queue_LED
             */
            status = xQueueSendToBack(Queue_LED, &msg, portMAX_DELAY);

        }

        // Delay for 10mS using vTaskDelay
        vTaskDelay(pdMS_TO_TICKS(10));
    }

}
