#!/bin/bash

# -- file names --
C_SOURCES="flappybird.c"
EXECUTABLE="flappybird"
NCURSES_FLAG="-lncurses"

# -- 1. Compilation --
echo "Compiling Flappy Bird game..."
if gcc -Wall -o $EXECUTABLE $C_SOURCES $NCURSES_FLAG;
then
	echo "Compilation successful - Your game is ready."
	# -- 2. Execution --
	echo "Press Enter to start the game."
	read # wait for the user to press a key
	
	./$EXECUTABLE

