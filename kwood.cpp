// Kenneth Wood
//Kenneth Wood's c++ personal file for 3350 project

#include "fonts.h"
#include <stdio.h>
#include <iostream>
using namespace std;
void show_kenneth_credits(int x, int y)
{
    Rect r;
    r.bot = y;
    r.left = x;
    r.center =1;
    ggprint8b(&r, 16, 0x00ffff34, "Kenneths work");
}

// Lab 10,working on quit button since it doesn't work
// and also fixed pacman's length in main game. Had to 
// cleanup sound as well on exit. Working on a new button 
// for credits as well.
int quitGame()
{
    cout << "quit was clicked!" <<endl;
    exit(0);
   
}

int toggleCredits(int credits)
{
    if(credits == 0)
        return 1;
    else 
        return 0;
}
