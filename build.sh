#!/bin/bash
gcc -lncurses main.c tetrimino.c -o ttetris -lm
#-lncurses for ncurses
#-lm for math.h