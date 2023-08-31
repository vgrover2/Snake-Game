/*
 * main.h
 *
 *  Created on: Dec 12, 2021
 *      Author: Spencer Sheahan
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "msp.h"
#include "msp432p401r.h"
#include "accelerometer.h"
#include "lightsensor.h"
#include "interrupts.h"
#include "lcd.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

/* RTOS header files */
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <Tasks/task_gameOver.h>
#include <Tasks/task_menuScreen.h>
#include <Tasks/task_moveSnake.h>
#include <Tasks/task_startGame.h>
#include <Tasks/task_toggleDarkMode.h>
#include <Tasks/task_toggleMotion.h>

// RTOS Task Handles and Queue Handles
extern TaskHandle_t TaskHandle_gameOver;
extern TaskHandle_t TaskHandle_menuScreen;
extern TaskHandle_t TaskHandle_moveSnake;
extern TaskHandle_t TaskHandle_startGame;
extern TaskHandle_t TaskHandle_toggleDarkMode;
extern TaskHandle_t TaskHandle_toggleMotion;
extern QueueHandle_t Queue_Snake_Pixels;

// Global variables
extern int pixels[64][64];

struct pixelCoord{
    uint8_t xCoord;
    uint8_t yCoord;
};
extern struct pixelCoord headPixel;
extern struct pixelCoord snackPixel;
extern struct pixelCoord tailPixel;

enum direction {up, down, left, right};
extern enum direction currDirection;
enum colors {snakeLight = 0xFE1F, snakeDark = 0x7D2F,
             snackLight = 0x49EF, snackDark = 0xC01F,
             background = 0x000F};
extern enum colors snakeColor;
extern enum colors snackColor;
extern enum colors backgroundColor;

extern uint8_t length;
extern uint8_t growthCounter;
extern bool gameIsOver;
extern bool motionControls;
extern bool lightMode;
extern bool menuScreen;

struct snakeInfo {
    enum direction currDirection;
    struct pixelCoord headPixel;
    struct pixelCoord tailPixel;
    enum colors color;
    uint8_t growthCounter;
    uint8_t length;
};
extern struct snakeInfo snake;

// Functions
void placeSnack(enum colors snackColor);
bool checkBoundary(enum direction currDirection);
bool checkSnake(enum direction currDirection);
bool checkSnack(enum direction currDirection);
void moveHeadForward(enum direction currDirection, enum colors snakeColor);
void moveTailForward(enum colors backgroundColor);

#endif /* MAIN_H_ */
