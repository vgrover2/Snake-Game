/*
 * task_startGame.c
 *
 *  Created on: Dec 13, 2021
 *      Author: Spencer Sheahan
 */

#include <Tasks/task_startGame.h>

TaskHandle_t TaskHandle_startGame;
int i;

/******************************************************************************
* Sets up the initial screen
******************************************************************************/
void task_startGame(void *pvParameters)
{
    struct pixelCoord snakeCoords;

    while(1)
    {
        // Do not want to restart in middle of game
        if(!gameIsOver){
            continue;
        }
        // Wait for ISR to let us know that the button
        // has been pressed
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        gameIsOver = false;
        menuScreen = false;

        // Set background
        int i, j;
        for(i = 0; i < 128; i++){
            for(j = 0; j < 128; j++){
                lcd_draw_pixel(i, j, backgroundColor);
            }
        }

        // Update snake data
        snake.tailPixel.xCoord = 25;
        snake.tailPixel.yCoord = 64;
        snake.headPixel.xCoord = 32;
        snake.headPixel.yCoord = 64;
        snake.growthCounter = 0;
        snake.currDirection = right;
        snake.length = 4;
        if(lightMode){
            snake.color = snakeLight;
        } else {
            snake.color = snakeDark;
        }

        // Add snake to the queue
        for(i = 0; i < 8; i++){
            snakeCoords.xCoord = 25 + i;
            snakeCoords.yCoord = 64;
            // Add pixels to snake map
            if(snakeCoords.xCoord % 2 == 0){
                pixels[snakeCoords.xCoord / 2][snakeCoords.yCoord] = 1;
            }
            lcd_draw_pixel(snakeCoords.xCoord, snakeCoords.yCoord, snake.color);
            xQueueSend(Queue_Snake_Pixels, &snakeCoords, portMAX_DELAY);
        }

        // Place snack
        if(lightMode){
            placeSnack(snackLight);
        } else {
            placeSnack(snackDark);
        }
    }
}
