#!/bin/bash
gcc main.c tetrimino.c -o ttetris_linux -lm -lncurses
#-lncurses for ncurses
#-lm for math.h
