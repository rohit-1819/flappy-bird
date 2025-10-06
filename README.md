Creating a functional Flappy Bird game using only shell script and C language for a terminal environment

Terminal Graphics - libraries required ncurses.h -(for a little fast terminal drawing)
Game Loop - The main function that runs repeatedly to update the game state. -- loop with a small delay(usleep)
Bird Physics - Gravity, jump action, velociy calculation
Pipe management - Generating, moving, and destroying the pipes -- Linked Lists to store pipe coordinates
Collision Detection - Checking if the bird hits a pipe or the ground -- coordinate comparison.
Score Tracking - Incrementing the score when the bird passes a pipe -- a count variable.

Compilation - gcc -o floppybird and floppybird.c -lncurses --Compiles your C source file (flappybird.c) into an executable (flappybird) and links the ncurses library.

Execution - ./floppybird 
Cleanup - rm -f floppybird  -- Removes the executable after playing


