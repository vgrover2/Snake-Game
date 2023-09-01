/*
 * task_gameOver.c
 *
 *  Created on: Dec 13, 2021
 *      Author: Vedant Grover
 */
#include "task_gameOver.h"

TaskHandle_t TaskHandle_gameOver;

void task_gameOver(void *pvParameters){
    while(1){
        // Wait for ISR to let us know that the button
        // has been pressed
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Buzzer function
        // Display Score
    }
}





