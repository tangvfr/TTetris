#include "main.h"
#include <curses.h>
#include <stdio.h>

WINDOW* win;
char** grill;
int h, w;
int tx, ty;
GameKey* keys;
Tetrimino* currentTetrimino;
Tetrimino* nextTetrimino;
char bx, by;
bool gameStart;
bool gameOver;
bool tetriminoPlaced;
long downTime;
long long downingTime;
unsigned long gameScore;
unsigned short gameLvl;
short confDownTime;
float cofDownTime;
unsigned int charTetriminoF;
unsigned int charTetriminoS;


int main()
{
	win = initscr();
	h = getmaxy(win);
	w = getmaxx(win);

	if (w < 42 || h < 22)
	{
		printw("Terminal is too small ! (min 42x22)");
		getch();
		endwin();
	}
	else
	{
		//min 42*22
		tx = w / 2 - 20;
		ty = h / 2 - 10;

		{
			//init
			confDownTime = 1000;
			cofDownTime = 0.9;
			createGameKey();
			loadConfig();
			currentTetrimino = createTetrimino();
			nextTetrimino = createTetrimino();
			createGrill();

			//init param
			nodelay(win, TRUE);
			keypad(win, TRUE);
			noecho();

			//init Color
			initColor();

			//init start
			setStart();

			while (gameStart)
			{
				while (!gameOver && gameStart)
				{
					//refrech and game key
					refreshGameKey();
					//waiting
					napms(50);
				
					//test game key
					gameKeyAction();//can place tetrimino

					//auto down
					if (!tetriminoPlaced)
						autoDown();
					
					//test tetriminoPlaced
					if (tetriminoPlaced)
					{
						if (canCurrentTetrimino(getBeginY(nextTetrimino), 5, nextTetrimino->rotate, nextTetrimino))
						{
							tetriminoPlaced = false;
							nextCurrentTetrimino();
						}
						else
							setGameOver();
					}

					//printing
					if (!gameOver)
					{
						printGrill();
						printTetrimino(ty + by, tx + bx * 2, currentTetrimino);
					}
				}
				//exit gameOver || !gameStart

				while (gameOver && gameStart)
				{
					refreshGameKey();

					//test key
					if (KEY_IS_DOWN(GAME_KEY_ESC))
						gameStart = false;
					else 
						if (oneKeyIsDown())
							initGame();//reset
				}
				//exit !gameOver || !gameStart
			}
			//exit !gameStart

			//bye msg
			byeMessage();

			//freeing
			freeTetrimino(currentTetrimino);
			freeTetrimino(nextTetrimino);
			freeGameKey();
			freeGrill();
			endwin();
		}
	}

	return 0;
}

void boxMessage(char* msg, int colorText)
{
	clearBox();
	drawBoxTetris();
	drawText(msg, 9, colorText);
}

void drawText(char* msg, char y, int colorText)
{
	int len = strlen(msg);
	move(ty + y, tx + 20 - (len / 2));
	
	attron(colorText);
	for (int i = 0; i < len; i++)
		addch(msg[i]);
	attroff(colorText);

	move(h - 1, w - 1);
}

void drawDetailBoxTetris()
{
	//middle bar
	move(ty, tx + 20);
	vline(ACS_VLINE, 20);
	addch(ACS_TTEE);
	move(ty + 20, tx + 20);
	addch(ACS_BTEE);

	textLegend(2, 29, "Next", FCOLOR_LIGHT_CYAN);
	separatorLegend(9);
	textLegend(12, 28, "Score", FCOLOR_LIGHT_YELLOW);
	textLegend(16, 28, "Level", FCOLOR_LIGHT_GREEN);
}

void drawBoxTetris()
{
	//top
	move(ty - 1, tx - 1);
	addch(ACS_ULCORNER);
	for (int i = 0; i < 16; i++)
	{
		addch(ACS_HLINE);
	}
	addch(ACS_RTEE);
	//T
	attron(COLOR_PAIR(FCOLOR_LIGHT_RED));
	addch('T');
	attroff(COLOR_PAIR(FCOLOR_LIGHT_RED));
	//E
	attron(COLOR_PAIR(FCOLOR_LIGHT_GREEN));
	addch('E');
	attroff(COLOR_PAIR(FCOLOR_LIGHT_GREEN));
	//T
	attron(COLOR_PAIR(FCOLOR_LIGHT_BLUE));
	addch('T');
	attroff(COLOR_PAIR(FCOLOR_LIGHT_BLUE));
	//R
	attron(COLOR_PAIR(FCOLOR_LIGHT_MAGENTA));
	addch('R');
	attroff(COLOR_PAIR(FCOLOR_LIGHT_MAGENTA));
	//I
	attron(COLOR_PAIR(FCOLOR_LIGHT_CYAN));
	addch('I');
	attroff(COLOR_PAIR(FCOLOR_LIGHT_CYAN));
	//S
	attron(COLOR_PAIR(FCOLOR_LIGHT_YELLOW));
	addch('S');
	attroff(COLOR_PAIR(FCOLOR_LIGHT_YELLOW));
	//end
	addch(ACS_LTEE);
	for (int i = 0; i < 16; i++)
	{
		addch(ACS_HLINE);
	}
	addch(ACS_URCORNER);

	//rigth
	move(ty, tx + 40);
	vline(ACS_VLINE, 20);
	move(ty + 20, tx + 40);
	addch(ACS_LRCORNER);

	//bottom
	move(ty + 20, tx - 1);
	addch(ACS_LLCORNER);
	hline(ACS_HLINE, 40);

	//left
	move(ty, tx - 1);
	vline(ACS_VLINE, 20);
}

void createGrill()
{
	grill = malloc(sizeof(char*) * 10);
	for (int i = 0; i < 10; i++)
	{
		grill[i] = malloc(sizeof(char) * 20);
	}
}

void printGrill()
{
	int color;
	for (int y = 0; y < 20; y++) 
	{
		move(ty + y, tx);
		for (int x = 0; x < 10; x++) 
		{
			color = COLOR_PAIR(grill[x][y]);
			attron(color);
			if (color != 0)
			{
				addch(charTetriminoF);
				addch(charTetriminoS);
			}
			else//for no color
			{
				addch(' ');
				addch(' ');
			}
			attroff(color);
		}
	}
}

void freeGrill()
{
	for (int i = 0; i < 10; i++)
	{
		free(grill[i]);
	}
	free(grill);
}

void createGameKey()
{
	keys = malloc(sizeof(GameKey) * SIZE_GAME_KEYS);
	for (int i = 0; i < SIZE_GAME_KEYS; i++)
	{
		keys[i].isDown = 0;
	}
	//init key
	keys[GAME_KEY_MOVE_LEFT].keyCode = 0x71;//q  
	keys[GAME_KEY_MOVE_RIGHT].keyCode = 0x64;//d  
	keys[GAME_KEY_MOVE_DOWN].keyCode = 0x73;//s  
	keys[GAME_KEY_ROTATE_LEFT].keyCode = 0x6B;//k  
	keys[GAME_KEY_FULL_DOWN].keyCode = 0x6C;//l  
	keys[GAME_KEY_ROTATE_RIGHT].keyCode = 0x6D;//m  
	keys[GAME_KEY_ESC].keyCode = 0x1B;//esc  
}

void refreshGameKey() 
{
	move(h - 1, w - 1);
	
	for (int i = 0; i < SIZE_GAME_KEYS; i++) 
	{
		keys[i].isDown = 0;
	}

	char ch;
	while ((ch = getch()) != ERR) 
	{
		int i = 0;
		while (i < SIZE_GAME_KEYS) 
		{
		if (keys[i].keyCode == ch)
		{
			keys[i].isDown = true;
			i = SIZE_GAME_KEYS;
		}
		i++;
		}
		// exit i >= SIZE_GAME_KEYS
	}
	// exit getch() == null
}

void freeGameKey()
{
	free(keys);
}

/* 0 >= y < 20 and 0 >= x < 10 and 0 >= rotate < 4*/
bool canCurrentTetrimino(char y, char x, char rotate, Tetrimino* tetrimino)
{
	bool collide = false; 
	int i = 0;
	Point* coord = tetrimino->coords[rotate];
	int tty, ttx;

	while (i < 4 && !collide)
	{
		tty = y + coord[i].y;
		ttx = x + coord[i].x;
		collide = 
			tty < 0
			|| tty >= 20
			|| ttx < 0
			|| ttx >= 10
			|| grill[ttx][tty] != 0
			;

		i++;
	}
	//exit i >= 4 || collide
	return !collide;
}

void tryRotate(char rotate) {
	if (canCurrentTetrimino(by, bx, rotate, currentTetrimino)) 
	{
    	currentTetrimino->rotate = rotate;
  	} 
	else
	{
		if (canCurrentTetrimino(by - 1, bx, rotate, currentTetrimino)) 
		{
			currentTetrimino->rotate = rotate;
			by--;
		} 
		else 
		{
			if (canCurrentTetrimino(by, bx - 1, rotate, currentTetrimino)) 
			{
				currentTetrimino->rotate = rotate;
				bx--;
			} 
			else 
			{
				if (canCurrentTetrimino(by, bx + 1, rotate, currentTetrimino)) 
				{
				addch(ACS_TTEE);
					currentTetrimino->rotate = rotate;
					bx++;
				} 
				else 
				{
					if (currentTetrimino->type == TETRIMINO_TYPE_I
					&& canCurrentTetrimino(by, bx + 2, rotate, currentTetrimino)
					) 
					{
						currentTetrimino->rotate = rotate;
						bx += 2;
					}
				};
			}
		}
	}
}

void nextCurrentTetrimino()
{
	copyTetrimino(currentTetrimino, nextTetrimino);
	setTetriminoRandom(nextTetrimino);
	by = getBeginY(currentTetrimino);
	bx = 5;

	//printing next
	for (char y = 0; y < 4; y++)
	{
		move(ty + 3 + y, tx + 28);
		for (char x = 0; x < 6; x++)
		{
			addch(' ');
		}
	}
	printTetrimino(ty + 5, tx + 30, nextTetrimino);
}

char getBeginY(Tetrimino* tetrimino)
{
	char by = 0;
	Point* coords = tetrimino->coords[tetrimino->rotate];
	
	for (int i = 0; i < 4; i++)
	{
		if (coords[i].y < by)
			by = coords[i].y;
	}

	return -by;
}

char getBottomY()
{
	char dy = by;
	char rotate = currentTetrimino->rotate;
	
	while (canCurrentTetrimino(dy, bx, rotate, currentTetrimino))
		dy++;
	dy--;

	return dy;
}

void printTetrimino(int y, int x, Tetrimino* tetrimino)
{
	int color = COLOR_PAIR(tetrimino->color);
	attron(color);
	Point* coords = tetrimino->coords[tetrimino->rotate];
	int dy;
	
	for (int i = 0; i < 4; i++)
	{
		dy = y + coords[i].y; 
		if (dy >= ty)
		{
			move(
				dy,
				x + coords[i].x * 2
				);
			addch(charTetriminoF);
			addch(charTetriminoS);
		}
	}
	attroff(color);
}

void gameKeyAction()
{
	if (KEY_IS_DOWN(GAME_KEY_ESC)) // esc
		gameStart = false;

	if (KEY_IS_DOWN(GAME_KEY_ROTATE_LEFT)) // rotate left
		tryRotate((unsigned)(currentTetrimino->rotate - 1) % 4);

	if (KEY_IS_DOWN(GAME_KEY_ROTATE_RIGHT)) // rotate right
		tryRotate((currentTetrimino->rotate + 1) % 4);

	if (KEY_IS_DOWN(GAME_KEY_MOVE_LEFT) // move left
		&& canCurrentTetrimino(by, bx - 1, currentTetrimino->rotate, currentTetrimino))
		bx--;

	if (KEY_IS_DOWN(GAME_KEY_MOVE_RIGHT) // move right
		&& canCurrentTetrimino(by, bx + 1, currentTetrimino->rotate, currentTetrimino))
		bx++;

	if (KEY_IS_DOWN(GAME_KEY_MOVE_DOWN) // move down
		&& canCurrentTetrimino(by + 1, bx, currentTetrimino->rotate, currentTetrimino))
	{
		by++;
		downingTime = getTime() + downTime;
		addScore(1);
	}

	if (KEY_IS_DOWN(GAME_KEY_FULL_DOWN)) // full down
	{
		char newY = getBottomY();
		addScore((newY - by) * 2);
		by = newY;
		placeTetrimino();
	}
}

void autoDown()
{
	long long time = getTime();
	if (time >= downingTime) {
		downingTime = time + downTime;

		if (canCurrentTetrimino(by + 1, bx, currentTetrimino->rotate, currentTetrimino))
			by++;
		else
			placeTetrimino();
	}
}	

void placeTetrimino()
{
	//placeTetrimino
	Point *coords = currentTetrimino->coords[currentTetrimino->rotate];
	char color = currentTetrimino->color;
	for (int i = 0; i < 4; i++) 
	{
		grill[bx + coords[i].x][by + coords[i].y] = color;
	}

	//check line placed
	bool line;
	char dx = 0;
	for (char dy = 0; dy < 20; dy++)
	{
		line = true;
		dx = 0;
		while (line && dx < 10)
		{
			if (grill[dx][dy] == 0)
				line = false;
			dx++;
		}

		if (line)
		{
			downLine(dy);
			addScore(100);
		}
	}

	//set placed
	tetriminoPlaced = true;
}

void downLine(char y)
{
	char x;
	for (; y > 1; y--)
	{
		for (x = 0; x < 10; x++)
		{
			grill[x][y] = grill[x][y - 1];
		}
	}
	//y <= 1 == 0
	for (x = 0; x < 10; x++)
	{
		grill[x][y] = 0;
	}
}

void initGame()
{
	gameScore = 0;
	gameStart = true;
	gameOver = false;
	tetriminoPlaced = false;
	downTime = confDownTime;
	downingTime = getTime() + downTime;
	setTetriminoRandom(nextTetrimino);

	//init grill
	for (int i = 0; i < 10; i++)
	{
		for (int i2 = 0; i2 < 20; i2++)
		{
			grill[i][i2] = COLOR_BLACK;
		}
	}

	clearBox();
	drawBoxTetris();
	drawDetailBoxTetris();
	nextCurrentTetrimino();
	addScore(0);
}

bool oneKeyIsDown()
{
	bool down = false;
	int i = 0;

	while (i < SIZE_GAME_KEYS && !down)
	{
		down = KEY_IS_DOWN(i);
		i++;
	}

	return down;
}

void setGameOver()
{
	gameOver = true;
	boxMessage("Game Over !", COLOR_PAIR(FCOLOR_LIGHT_RED));
	char score[40] = {};
	sprintf(score, "Score %lu", gameScore);
	drawText(score, 10, COLOR_PAIR(FCOLOR_LIGHT_YELLOW));

	refresh();
	napms(250);
	refreshGameKey();
}

void setStart()
{
	gameOver = true;
	gameStart = true;
	boxMessage("Tetris by Tangv", COLOR_PAIR(FCOLOR_LIGHT_MAGENTA));
	drawText("Press a key", 10, COLOR_PAIR(FCOLOR_LIGHT_CYAN));
	refresh();
	napms(250);
	refreshGameKey();
}

void byeMessage()
{
	boxMessage("Exit", COLOR_PAIR(FCOLOR_LIGHT_BLUE));
	drawText("Good bye", 10, COLOR_PAIR(FCOLOR_LIGHT_GREEN));
	refresh();
	napms(1500);
}

void addScore(unsigned long score)
{
	gameScore += score;
	attron(COLOR_PAIR(FCOLOR_LIGHT_YELLOW));
	mvprintw(
		ty + 13,
		tx + 30 - (lenNumber(gameScore, 10)) / 2,
		"%lu",
		gameScore
	);
	attroff(COLOR_PAIR(FCOLOR_LIGHT_YELLOW));
	setLevel();
}

void setLevel()
{
	gameLvl = 1 + gameScore / 1000;
	downTime = (long) (confDownTime * pow(
										cofDownTime,
										(double) (gameLvl - 1)
										)
									);
	attron(COLOR_PAIR(FCOLOR_LIGHT_GREEN));
	mvprintw(
		ty + 17,
		tx + 30 - (lenNumber(gameLvl, 10)) / 2,
		"%u",
		gameLvl
	);
	attroff(COLOR_PAIR(FCOLOR_LIGHT_GREEN));
}


void initColor() 
{
	if (has_colors()) 
	{
		start_color();
		charTetriminoF = '<';
		charTetriminoS = '>';
	} 
	else
	{
		charTetriminoF = ACS_CKBOARD;
		charTetriminoS = ACS_CKBOARD;
	}

	initColorPair(COLOR_BLACK);
	initColorPair(COLOR_RED);
	initColorPair(COLOR_GREEN);
	initColorPair(COLOR_YELLOW);
	initColorPair(COLOR_BLUE);
	initColorPair(COLOR_MAGENTA);
	initColorPair(COLOR_CYAN);
	initColorPair(COLOR_WHITE);
	initColorPair(COLOR_LIGHT_BLACK);
	initColorPair(COLOR_LIGHT_RED);
	initColorPair(COLOR_LIGHT_GREEN);
	initColorPair(COLOR_LIGHT_YELLOW);
	initColorPair(COLOR_LIGHT_BLUE);
	initColorPair(COLOR_LIGHT_MAGENTA);
	initColorPair(COLOR_LIGHT_CYAN);
	initColorPair(COLOR_LIGHT_WHITE);

	initFColorPair(FCOLOR_BLACK);
	initFColorPair(FCOLOR_LIGHT_BLACK);
	initFColorPair(FCOLOR_RED);
	initFColorPair(FCOLOR_LIGHT_RED);
	initFColorPair(FCOLOR_GREEN);
	initFColorPair(FCOLOR_LIGHT_GREEN);
	initFColorPair(FCOLOR_YELLOW);
	initFColorPair(FCOLOR_LIGHT_YELLOW);
	initFColorPair(FCOLOR_BLUE);
	initFColorPair(FCOLOR_LIGHT_BLUE);
	initFColorPair(FCOLOR_MAGENTA);
	initFColorPair(FCOLOR_LIGHT_MAGENTA);
	initFColorPair(FCOLOR_CYAN);
	initFColorPair(FCOLOR_LIGHT_CYAN);
	initFColorPair(FCOLOR_WHITE);
	initFColorPair(FCOLOR_LIGHT_WHITE);
}

void clearBox()
{
	for (int i = -1; i < 21; i++)
	{
		move(ty + i, tx - 1);
		hline(' ', 42);
	}
}

char lenNumber(unsigned long num, unsigned char base)
{
	char len = 0;
	while (num > 0)
	{
		len++;
		num /= base;
	}
	//exit num <= 0

	return (len == 0) ? 1 : len;
}

long long getTime() 
{
    struct timeval te; 
    gettimeofday(&te, NULL);
    return te.tv_sec*1000LL + te.tv_usec/1000;
}

void separatorLegend(char y)
{
	move(ty + y, tx + 20);
	hline(ACS_HLINE, 20);
	addch(ACS_LTEE);
	move(ty + y, tx + 40);
	addch(ACS_RTEE);
}

void textLegend(char y, char x, char* str, int color)
{
	attron(COLOR_PAIR(color));
	mvprintw(
		ty + y,
		tx + x,
		str
		);
	attroff(COLOR_PAIR(color));
}

void loadConfig() 
{
	FILE* conf = NULL;
	conf = fopen(CONFIG_FILE, "r");

	if (conf != NULL)
	{
		GameConf gc;
		if (fread(&gc, sizeof(GameConf), 1, conf))
		{
			confDownTime = gc.confDownTime;
			cofDownTime = gc.cofDownTime;
			keys[GAME_KEY_MOVE_LEFT].keyCode = gc.moveLeft;
			keys[GAME_KEY_MOVE_RIGHT].keyCode = gc.moveRight;
			keys[GAME_KEY_MOVE_DOWN].keyCode = gc.moveDown;
			keys[GAME_KEY_ROTATE_LEFT].keyCode = gc.rotateLeft;
			keys[GAME_KEY_FULL_DOWN].keyCode = gc.fullDown;
			keys[GAME_KEY_ROTATE_RIGHT].keyCode = gc.rotateRight;
			keys[GAME_KEY_ESC].keyCode = gc.esc;
		}
		else 
		{
			printf("Error to read config !");
		}

		if (fclose(conf) == EOF)
		{
			printf("Error to close conf file !");
		}
	}
	else 
	{
		exportConfig();
	}
}

void exportConfig()
{
	FILE* conf = NULL;
	conf = fopen(CONFIG_FILE, "w");

	if (conf != NULL)
	{
		GameConf gc;
		gc.confDownTime = confDownTime;
		gc.cofDownTime = cofDownTime;
		gc.moveLeft = keys[GAME_KEY_MOVE_LEFT].keyCode;
		gc.moveRight = keys[GAME_KEY_MOVE_RIGHT].keyCode;
		gc.moveDown = keys[GAME_KEY_MOVE_DOWN].keyCode;
		gc.rotateLeft = keys[GAME_KEY_ROTATE_LEFT].keyCode;
		gc.fullDown = keys[GAME_KEY_FULL_DOWN].keyCode;
		gc.rotateRight = keys[GAME_KEY_ROTATE_RIGHT].keyCode;
		gc.esc = keys[GAME_KEY_ESC].keyCode;

		if (fwrite(&gc, sizeof(GameConf), 1, conf))
		{
			printf("Error config is not set !");
		}

		if (fclose(conf) == EOF)
		{
			printf("Error to close conf file !");
		}
	}
	else 
	{
		printf("Error to open conf file !");
	}
}