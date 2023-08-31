/*
 * task_moveSnake.c
 *
 *  Created on: Dec 13, 2021
 *      Author: Spencer Sheahan
 */
#include "task_moveSnake.h"

#define VOLT_0P85 ((int)(0.85/(3.3/4096))) // 1056      // 0.85 /(3.3/4096)
#define VOLT_2P50 ((int)(2.50/(3.3/4096))) // 3103      // 2.50 /(3.3/4096)

TaskHandle_t TaskHandle_moveSnake;

void task_moveSnake(void *pvParameters)
{
    while(1){
        // Wait for ISR to let us know that the game has started
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        if(gameIsOver || menuScreen){
        } else {
            // Calculate current direction
            getCurrDirection();

            // Call detect game over function
            // If true, continue and set gameIsOver to true
            if(checkBoundary(snake.currDirection)){
                gameIsOver = true;
            }
            if(checkSnake(snake.currDirection)){
                gameIsOver = true;
            }

            // Check for snack function (returns true if food)
            // If true, add 4 to growth counter and replace food
            if(checkSnack(snake.currDirection)){
                snake.growthCounter += 4;
                placeSnack(snackColor);
            }

            // Add to front function
            moveHeadForward(snake.currDirection, snake.color);
            // If growth counter = 0, remove from back function
            // Else, growth counter--
            if(snake.growthCounter > 0){
                snake.growthCounter--;
                snake.length++;
            } else {
                moveTailForward(backgroundColor);
            }
        }
    }
}

/**************************************************
 * \brief Get direction that the snake is moving
 *
 * \param none
 *
 * \return none
 *************************************************/
void getCurrDirection(){
    if(motionControls){
        if(ACCEL_X_DIR > VOLT_2P50){
            // Move left
            snake.currDirection = left;
        } else if (ACCEL_X_DIR < VOLT_0P85){
            // Move right
            snake.currDirection = right;
        } else if (ACCEL_Y_DIR > VOLT_2P50){
            // Move up
            snake.currDirection = up;
        } else if (ACCEL_Y_DIR < VOLT_0P85){
            // Move down
            snake.currDirection = down;
        } else {
            // Continue in the same direction
        }
    } else {
        if(PS2_X_DIR > VOLT_2P50){
            // Move left
            snake.currDirection = left;
        } else if (PS2_X_DIR < VOLT_0P85){
            // Move right
            snake.currDirection = right;
        } else if (PS2_Y_DIR > VOLT_2P50){
            // Move up
            snake.currDirection = up;
        } else if (PS2_Y_DIR < VOLT_0P85){
            // Move down
            snake.currDirection = down;
        } else {
            // Continue in the same direction
        }
    }

}
