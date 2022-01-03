#pragma once
#include "../../Display/include/ESP32ILI9341.h"
#include "../../GlobalDefine.h"

//store snake position
//head is 0
int16_t nextHeadX;
int16_t nextHeadY;
uint16_t snakeX[mapWidht];
uint16_t snakeY[mapHeight];

//store everything, temprary walls, bonus, food, etc.
uint16_t map[mapWidht][mapHeight];

int score;
int size;

enum dir
{
    up = 0,
    right,
    down,
    left,
    none
};

bool ate;

enum dir SNAKE_getDir();
enum dir SNAKE_dirCheck(enum dir current, enum dir previous);

void SNAKE_gameStart(spi_device_handle_t spi);

void SNAKE_mainLoop(spi_device_handle_t spi);

void SNAKE_updateHead(enum dir direction);
bool SNAKE_Move();

bool SNAKE_IsFood();
void SNAKE_generateFood();
void SNAKE_sendScreen(spi_device_handle_t spi);
bool SNAKE_gameOver();

//endless loop
void SNAKE_gameEnded();
