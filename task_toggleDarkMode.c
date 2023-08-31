/*
 * task_toggleDarkMode.c
 *
 *  Created on: Dec 14, 2021
 *      Author: Spencer Sheahan
 */

#include "task_toggleDarkMode.h"

TaskHandle_t TaskHandle_toggleDarkMode;

void task_toggleDarkMode(void *pvParameters){
    while(1){
        // Wait for ISR to let us know that the button
        // has been pressed
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
            if(opt3001_read_light() < 3000){
                lightMode = false;
                snake.color = snakeDark;
                snackColor = snackDark;
            } else if (opt3001_read_light() > 3000){
                lightMode = true;
                snake.color = snakeLight;
                snackColor = snackLight;
            }
    }
}


// Based on Light
