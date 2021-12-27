#ifndef __TETRIMINO
#define __TETRIMINO

#include <curses.h>
#include <stdlib.h>
#include "color.h"

#define __TetriminoSetCoord(coord, c1p, c1s, c2p, c2s, c3p, c3s, c4p, c4s) \
                                                                            coord[0].x = c1p; \
                                                                            coord[0].y = c1s; \
                                                                            coord[1].x = c2p; \
                                                                            coord[1].y = c2s; \
                                                                            coord[2].x = c3p; \
                                                                            coord[2].y = c3s; \
                                                                            coord[3].x = c4p; \
                                                                            coord[3].y = c4s

#define __TetriminoCopyCoord(coord, copy) \
                                                                            coord[0].x = copy[0].x; \
                                                                            coord[0].y = copy[0].y; \
                                                                            coord[1].x = copy[1].x; \
                                                                            coord[1].y = copy[1].y; \
                                                                            coord[2].x = copy[2].x; \
                                                                            coord[2].y = copy[2].y; \
                                                                            coord[3].x = copy[3].x; \
                                                                            coord[3].y = copy[3].y
                                                                            
#define TETRIMINO_TYPE_I 0
#define TETRIMINO_TYPE_O 1
#define TETRIMINO_TYPE_T 2
#define TETRIMINO_TYPE_L 3
#define TETRIMINO_TYPE_J 4
#define TETRIMINO_TYPE_Z 5
#define TETRIMINO_TYPE_S 6

typedef struct Point Point;
struct Point
{
    char x;
    char y;
};

typedef struct Tetrimino Tetrimino;
struct Tetrimino {
    char color;
    Point** coords;
    char rotate;
    char type;
};

Tetrimino* createTetrimino();
void freeTetrimino(Tetrimino* tetrimino);
void setTetriminoRandom(Tetrimino* tetrimino);
void setTetriminoI(Tetrimino* tetrimino);
void setTetriminoO(Tetrimino* tetrimino);
void setTetriminoT(Tetrimino* tetrimino);
void setTetriminoL(Tetrimino* tetrimino);
void setTetriminoJ(Tetrimino* tetrimino);
void setTetriminoZ(Tetrimino* tetrimino);
void setTetriminoS(Tetrimino* tetrimino);
void copyTetrimino(Tetrimino* tetrimino, Tetrimino* copyed);

#endif