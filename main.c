/*
 * Copyright (c) 2016-2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== main_freertos.c ========
 */
#include "main.h"

#define VOLT_0P85 ((int)(0.85/(3.3/4096))) // 1056      // 0.85 /(3.3/4096)
#define VOLT_2P50 ((int)(2.50/(3.3/4096))) // 3103      // 2.50 /(3.3/4096)

TaskHandle_t Task_Blink_LED1_Handle = NULL;
QueueHandle_t Queue_Snake_Pixels;
int pixels[64][64];
struct pixelCoord snackPixel;
struct snakeInfo snake;
enum colors snakeColor;
enum colors snackColor;
enum colors backgroundColor;
bool gameIsOver;
bool motionControls;
bool lightMode;
bool menuScreen;
enum direction currDirection;


/* ****************************************************************************
 * \brief   This Function initializes the hardware required to run snake
 *
 * \param   none
 *
 * \return  none
 * ***************************************************************************/
void snake_init(void)
{
    accel_init();
    i2c_init();
    opt3001_init();
    lcd_init();
    init_s1_MKII();
    init_s2_MKII();
    ece353_ADC14_PS2_XY();
    ece353_T32_1_Interrupt_Ms(50);
}

/******************************************************************************
 * \brief   Places a snack in an unoccupied pixel
 *
 * \param   snackColor - the color of the snack to be placed
 *
 * \return  none
 *****************************************************************************/
void placeSnack(enum colors snackColor){
    while(1){
        uint8_t xRand = rand() % 64;
        uint8_t yRand = rand() % 64;
        // Check if pixel is unoccupied
        if(pixels[xRand][yRand] == 0){
            lcd_draw_pixel(xRand * 2, yRand * 2, snackColor);
            snackPixel.xCoord = xRand * 2;
            snackPixel.yCoord = yRand * 2;
            break;
        }
    }
}

/******************************************************************************
 * \brief checks if the snake is at the boundary of the screen
 *
 * \param currDirection - the direction that the snake is moving
 *
 * \return true if boundary is found, false if not
 *****************************************************************************/
bool checkBoundary(enum direction currDirection){
    if(snake.headPixel.xCoord <= 0 && currDirection == left){
        return true;
    } else if (snake.headPixel.xCoord >= 126 && currDirection == right){
        return true;
    } else if (snake.headPixel.yCoord <= 0 && currDirection == up){
        return true;
    } else if (snake.headPixel.yCoord >= 126 && currDirection == down){
        return true;
    } else {
        return false;
    }
}

/******************************************************************************
 * \brief checks if the snake will run into itself
 *
 * \param currDirection - the direction that the snake is moving
 *
 * \return true if snake is found, false if not
 *****************************************************************************/
bool checkSnake(enum direction currDirection){
    if(currDirection == left){
        if(pixels[(snake.headPixel.xCoord - 2) / 2][snake.headPixel.yCoord / 2] != 0 ){
            return true;
        }
    } else if (currDirection == right){
        if(pixels[(snake.headPixel.xCoord + 2) / 2][snake.headPixel.yCoord / 2] != 0){
            return true;
        }
    } else if (currDirection == down){
        if(pixels[snake.headPixel.xCoord / 2][(snake.headPixel.yCoord + 2) / 2] != 0){
            return true;
        }
    } else if (currDirection == up){
        if(pixels[snake.headPixel.xCoord / 2][(snake.headPixel.yCoord - 2) / 2] != 0){
            return true;
        }
    }
    return false;
}

/******************************************************************************
 * \brief checks if the snake is at the snack
 *
 * \param none
 *
 * \return none
 *****************************************************************************/
bool checkSnack(enum direction currDirection){
    if(currDirection == left){
        if(((snake.headPixel.xCoord - 2) == snackPixel.xCoord) && (snake.headPixel.yCoord == snackPixel.yCoord)){
            return true;
        }
    } else if (currDirection == right){
        if(((snake.headPixel.xCoord + 2) == snackPixel.xCoord) && (snake.headPixel.yCoord == snackPixel.yCoord)){
            return true;
        }
    } else if (currDirection == down){
        if(((snake.headPixel.xCoord) == snackPixel.xCoord) && ((snake.headPixel.yCoord + 2) == snackPixel.yCoord)){
            return true;
        }
    } else if (currDirection == up){
        if(((snake.headPixel.xCoord) == snackPixel.xCoord) && ((snake.headPixel.yCoord - 2) == snackPixel.yCoord)){
            return true;
        }
    }
    return false;
}

/******************************************************************************
 * \brief Moves the snake's head forward to the next pixel
 *
 * \param currDirection - The direction the snake is going
 *        snakeColor    - The color of the snake
 *
 * \return none
 *****************************************************************************/
void moveHeadForward(enum direction currDirection, enum colors snakeColor){
    int i;
    for(i = 0; i < 2; i++){
        if(currDirection == left){
            snake.headPixel.xCoord -= 1;
        } else if (currDirection == right){
            snake.headPixel.xCoord += 1;
        } else if(currDirection == up){
            snake.headPixel.yCoord -= 1;
        } else if (currDirection == down){
            snake.headPixel.yCoord += 1;
        }

        // Update snake map
        if(((snake.headPixel.xCoord % 2) == 0) && ((snake.headPixel.yCoord % 2) == 0)){
            pixels[snake.headPixel.xCoord / 2][snake.headPixel.yCoord / 2] = 1;
        }
        // Sent pixel to queue
        xQueueSend(Queue_Snake_Pixels, &snake.headPixel, portMAX_DELAY);
        // Draw pixel on screen
        lcd_draw_pixel(snake.headPixel.xCoord, snake.headPixel.yCoord, snake.color);
    }
    return;
}

/******************************************************************************
 * \brief Moves snake's tail forward to the next pixel
 *
 * \param   backgroundColor - the color of the background
 *
 * \return  none
 *****************************************************************************/
void moveTailForward(enum colors backgroundColor){
    int i;
    for(i = 0; i < 2; i++){
        // Remove from queue
        xQueueReceive(Queue_Snake_Pixels, &snake.tailPixel, portMAX_DELAY);
        // Remove from screen
        lcd_draw_pixel(snake.tailPixel.xCoord, snake.tailPixel.yCoord, backgroundColor);
        // Update snake map
        if(((snake.tailPixel.xCoord % 2) == 0) && ((snake.tailPixel.yCoord % 2) == 0)){
            pixels[snake.tailPixel.xCoord / 2][snake.tailPixel.yCoord / 2] = 0;
        }
    }
    return;
}

/*
 *  ======== main ========
 */
int main(void)
 {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    // Initialize peripherals
    snake_init();

    // We will want the queue to hold the snake pixels
    Queue_Snake_Pixels = xQueueCreate(256, sizeof(struct pixelCoord));

    // Tasks
    xTaskCreate
    (   task_gameOver,
        "task_gameOver",
        configMINIMAL_STACK_SIZE,
        NULL,
        1,
        &TaskHandle_gameOver
    );

    xTaskCreate
    (   task_moveSnake,
        "task_moveSnake",
        configMINIMAL_STACK_SIZE,
        NULL,
        1,
        &TaskHandle_moveSnake
    );

    xTaskCreate
    (   task_toggleMotion,
        "task_toggleMotion",
        configMINIMAL_STACK_SIZE,
        NULL,
        1,
        &TaskHandle_toggleMotion
    );

    xTaskCreate
    (   task_startGame,
        "task_startGame",
        configMINIMAL_STACK_SIZE,
        NULL,
        1,
        &TaskHandle_startGame
    );

    xTaskCreate
    (   task_toggleDarkMode,
        "task_toggleDarkMode",
        configMINIMAL_STACK_SIZE,
        NULL,
        1,
        &TaskHandle_toggleDarkMode
    );

    xTaskCreate
    (   task_menuScreen,
        "task_menuScreen",
        configMINIMAL_STACK_SIZE,
        NULL,
        1,
        &TaskHandle_menuScreen
    );

    // Initialize certain values
    gameIsOver = true;
    motionControls = false;
    lightMode = true;
    menuScreen = true;

    // Display menu screen


    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler();

    return (0);
}


//*****************************************************************************
//
//! \brief Application defined malloc failed hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationMallocFailedHook()
{
    /* Handle Memory Allocation Errors */
    while(1)
    {
    }
}

//*****************************************************************************
//
//! \brief Application defined stack overflow hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    //Handle FreeRTOS Stack Overflow
    while(1)
    {
    }
}
