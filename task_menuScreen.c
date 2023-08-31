/*
 * task_menuScreen.c
 *
 *  Created on: Dec 13, 2021
 *      Author: Spencer Sheahan
 */

/*
 * task_startGame.c
 *
 *  Created on: Dec 14, 2021
 *      Author: Spencer Sheahan
 */

#include <main.h>
#include <Tasks/task_menuScreen.h>

TaskHandle_t TaskHandle_menuScreen;

void task_menuScreen(void *pvParameters)
{
    while(1){
        // Wait for ISR to let us know that the button
        // has been pressed
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Flash menu screen to LCD
    }

}

