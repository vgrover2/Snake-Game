/*
 * task_toggleMotion.c
 *
 *  Created on: Dec 14, 2021
 *      Author: Spencer Sheahan
 */

#include "task_toggleMotion.h"

TaskHandle_t TaskHandle_toggleMotion;

void task_toggleMotion(void *pvParameters){
    while(1){
        // Wait for ISR to let us know that the button
        // has been pressed
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        motionControls = !motionControls;
        ece353_T32_1_wait_100mS();
    }
}


