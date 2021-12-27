#include "tetrimino.h"

Tetrimino* createTetrimino()
{
    Tetrimino* tetrimino = malloc(sizeof(Tetrimino));
    tetrimino->coords = malloc(sizeof(Point*) * 4);
    for (int r = 0; r < 4; r++)
    {
        tetrimino->coords[r] = malloc(sizeof(Point) * 4);
    }
    return tetrimino;
}


void setTetriminoRandom(Tetrimino* tetrimino)
{
    switch (rand() % 7)
    {
        case 0:
            setTetriminoI(tetrimino);
            break;
            
        case 1:
            setTetriminoO(tetrimino);
            break;
            
        case 2:
            setTetriminoT(tetrimino);
            break;
            
        case 3:
            setTetriminoS(tetrimino);
            break;
            
        case 4:
            setTetriminoZ(tetrimino);
            break;
            
        case 5:
            setTetriminoL(tetrimino);
            break;

        default:
            setTetriminoJ(tetrimino);
            break;
    }
}

void setTetriminoI(Tetrimino* tetrimino)
{
    tetrimino->color = COLOR_LIGHT_CYAN;
    tetrimino->rotate = 0;
    tetrimino->type = TETRIMINO_TYPE_I;
    Point* coords;
    coords = tetrimino->coords[0];
    __TetriminoSetCoord(coords, 0, -2, 0, -1, 0, 0, 0, 1);
    coords = tetrimino->coords[1];
    __TetriminoSetCoord(coords, -2, 0, -1, 0, 0, 0, 1, 0);
    coords = tetrimino->coords[2];
    __TetriminoSetCoord(coords, 0, -2, 0, -1, 0, 0, 0, 1);
    coords = tetrimino->coords[3];
    __TetriminoSetCoord(coords, -2, 0, -1, 0, 0, 0, 1, 0);
    /*
    (0, -2), (0, -1), (0, 0), (0, 1), 
    (-2, 0), (-1, 0), (0, 0), (1, 0), 
    */
}

void setTetriminoO(Tetrimino* tetrimino)
{
    tetrimino->color = COLOR_LIGHT_YELLOW;
    tetrimino->rotate = 0;
    tetrimino->type = TETRIMINO_TYPE_O;
    Point* coords;
    coords = tetrimino->coords[0];
    __TetriminoSetCoord(coords, -1, -1, 0, -1, -1, 0, 0, 0);
    coords = tetrimino->coords[1];
    __TetriminoSetCoord(coords, -1, -1, 0, -1, -1, 0, 0, 0);
    coords = tetrimino->coords[2];
    __TetriminoSetCoord(coords, -1, -1, 0, -1, -1, 0, 0, 0);
    coords = tetrimino->coords[3];
    __TetriminoSetCoord(coords, -1, -1, 0, -1, -1, 0, 0, 0);
    /*
    (-1, -1), (0, -1), (-1, 0), (0, 0), 
    */
}

void setTetriminoT(Tetrimino* tetrimino)
{
    tetrimino->color = COLOR_LIGHT_MAGENTA;
    tetrimino->rotate = 0;
    tetrimino->type = TETRIMINO_TYPE_T;
    Point* coords;
    coords = tetrimino->coords[0];
    __TetriminoSetCoord(coords, 1, 0, 0, -1, -1, 0, 0, 0);
    coords = tetrimino->coords[1];
    __TetriminoSetCoord(coords, 0, 1, 1, 0, 0, -1, 0, 0);
    coords = tetrimino->coords[2];
    __TetriminoSetCoord(coords, -1, 0, 0, 1, 1, 0, 0, 0);
    coords = tetrimino->coords[3];
    __TetriminoSetCoord(coords, 0, -1, -1, 0, 0, 1, 0, 0);
    /*
    (1, 0), (0, -1), (-1, 0), (0, 0),
    (0, 1), (1, 0), (0, -1), (0, 0),
    (-1, 0), (0, 1), (1, 0), (0, 0), 
    (0, -1), (-1, 0), (0, 1), (0, 0), 
    */
}

void setTetriminoL(Tetrimino* tetrimino)
{
    tetrimino->color = COLOR_YELLOW;
    tetrimino->rotate = 0;
    tetrimino->type = TETRIMINO_TYPE_L;
    Point* coords;
    coords = tetrimino->coords[0];
    __TetriminoSetCoord(coords, 0, -1, 0, 0, 0, 1, 1, 1);
    coords = tetrimino->coords[1];
    __TetriminoSetCoord(coords, 1, 0, 0, 0, -1, 0, -1, 1);
    coords = tetrimino->coords[2];
    __TetriminoSetCoord(coords, 0, 1, 0, 0, 0, -1, -1, -1);
    coords = tetrimino->coords[3];
    __TetriminoSetCoord(coords, -1, 0, 0, 0, 1, 0, 1, -1);
    /*
    (0, -1), (0, 0), (0, 1), (1, 1),
    (1, 0), (0, 0), (-1, 0), (-1, 1),
    (0, 1), (0, 0), (0, -1), (-1, -1),
    (-1, 0), (0, 0), (1, 0), (1, -1), 
    */
}

void setTetriminoJ(Tetrimino* tetrimino)
{
    tetrimino->color = COLOR_LIGHT_BLUE;
    tetrimino->rotate = 0;
    tetrimino->type = TETRIMINO_TYPE_J;
    Point* coords;
    coords = tetrimino->coords[0];
    __TetriminoSetCoord(coords, 0, -1, 0, 0, 0, 1, -1, 1);
    coords = tetrimino->coords[1];
    __TetriminoSetCoord(coords, 1, 0, 0, 0, -1, 0, -1, -1);
    coords = tetrimino->coords[2];
    __TetriminoSetCoord(coords, 0, 1, 0, 0, 0, -1, 1, -1);
    coords = tetrimino->coords[3];
    __TetriminoSetCoord(coords, -1, 0, 0, 0, 1, 0, 1, 1);
    /*
    (0, -1), (0, 0), (0, 1), (-1, 1),
    (1, 0), (0, 0), (-1, 0), (-1, -1),
    (0, 1), (0, 0), (0, -1), (1, -1),
    (-1, 0), (0, 0), (1, 0), (1, 1),
    */
}

void setTetriminoZ(Tetrimino* tetrimino)
{
    tetrimino->color = COLOR_LIGHT_RED;
    tetrimino->rotate = 0;
    tetrimino->type = TETRIMINO_TYPE_Z;
    Point* coords;
    coords = tetrimino->coords[0];
    __TetriminoSetCoord(coords, -1, -1, 0, -1, 0, 0, 1, 0);
    coords = tetrimino->coords[1];
    __TetriminoSetCoord(coords, 1, -1, 1, 0, 0, 0, 0, 1);
    coords = tetrimino->coords[2];
    __TetriminoSetCoord(coords, -1, -1, 0, -1, 0, 0, 1, 0);
    coords = tetrimino->coords[3];
    __TetriminoSetCoord(coords, 1, -1, 1, 0, 0, 0, 0, 1);
    /*
    (-1, -1), (0, -1), (0, 0), (1, 0),
    (1, -1), (1, 0), (0, 0), (0, 1), 
    */
}

void setTetriminoS(Tetrimino* tetrimino)
{
    tetrimino->color = COLOR_LIGHT_GREEN;
    tetrimino->rotate = 0;
    tetrimino->type = TETRIMINO_TYPE_S;
    Point* coords;
    coords = tetrimino->coords[0];
    __TetriminoSetCoord(coords, 1, -1, 0, -1, 0, 0, -1, 0);
    coords = tetrimino->coords[1];
    __TetriminoSetCoord(coords, 1, 1, 1, 0, 0, 0, 0, -1);
    coords = tetrimino->coords[2];
    __TetriminoSetCoord(coords, 1, -1, 0, -1, 0, 0, -1, 0);
    coords = tetrimino->coords[3];
    __TetriminoSetCoord(coords, 1, 1, 1, 0, 0, 0, 0, -1);
    /*
    (1, -1), (0, -1), (0, 0), (-1, 0),
    (1, 1), (1, 0), (0, 0), (0, -1),
    */
}

void freeTetrimino(Tetrimino* tetrimino)
{
    for (int r = 0; r < 4; r++)
    {
        free(tetrimino->coords[r]);
    }
    free(tetrimino->coords);
    free(tetrimino);
}

void copyTetrimino(Tetrimino* tetrimino, Tetrimino* copyed)
{
    tetrimino->color = copyed->color;
    tetrimino->rotate = copyed->rotate;
    Point* coords;
    Point* copy;

    coords = tetrimino->coords[0];
    copy = copyed->coords[0];
    __TetriminoCopyCoord(coords, copy);

    coords = tetrimino->coords[1];
    copy = copyed->coords[1];
    __TetriminoCopyCoord(coords, copy);

    coords = tetrimino->coords[2];
    copy = copyed->coords[2];
    __TetriminoCopyCoord(coords, copy);
    
    coords = tetrimino->coords[3];
    copy = copyed->coords[3];
    __TetriminoCopyCoord(coords, copy);
}