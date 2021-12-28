#ifndef __TETRIS
#define __TETRIS

#include <curses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include "tetrimino.h"
#include "color.h"

#define CONFIG_FILE "./.conf"

#define initColorPair(color) init_pair(color, color, color)
#define initFColorPair(color) init_pair(color, 0xF & color, COLOR_BLACK)

#define GAME_KEY_MOVE_LEFT 0
#define GAME_KEY_MOVE_RIGHT 1 
#define GAME_KEY_MOVE_DOWN 2
#define GAME_KEY_ROTATE_LEFT 3
#define GAME_KEY_ROTATE_RIGHT 4
#define GAME_KEY_FULL_DOWN 5
#define GAME_KEY_ESC 6
#define SIZE_GAME_KEYS 7

#define KEY_IS_DOWN(key) keys[key].isDown

typedef struct GameKey GameKey;
struct GameKey
{
	char keyCode;
	bool isDown;
};

typedef struct GameConf GameConf;
struct GameConf
{
	short confDownTime;
	float cofDownTime;
	char moveLeft;
	char moveRight;
	char moveDown;
	char rotateLeft;
	char fullDown;
	char rotateRight;
	char esc;
};

int main();
void boxMessage(char* msg, int colorPairText);
void drawText(char* msg, char y, int colorPairText);
void drawDecoBoxTetris();
void drawBoxTetris();
void createGrill();
void printGrill();
void freeGrill();
void nextCurrentTetrimino();
void printTetrimino(int y, int x, Tetrimino* tetrimino);
bool canCurrentTetrimino(char y, char x, char rotate, Tetrimino* tetrimino);
char getBeginY(Tetrimino* tetrimino);
char getBottomY();
void tryRotate(char rotate);
void createGameKey();
void refreshGameKey();
void freeGameKey();
void gameKeyAction();
void autoDown();
void placeTetrimino();
void downLine(char y);
void initGame();
bool oneKeyIsDown();
void setGameOver();
void setStart();
void byeMessage();
void addScore(unsigned long score);
void setLevel();
void initColor();
void clearBox();
char lenNumber(unsigned long num, unsigned char base);
long long getTime();
void separatorLegend(char y);
void textLegend(char y, char x, char* str, int pairColor);
void loadConfig();
void exportConfig();

#endif


