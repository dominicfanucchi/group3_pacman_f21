// Kenneth Wood
//Kenneth Wood's c++ personal file for 3350 project

#include "fonts.h"
#include <stdio.h>
#include <iostream>
using namespace std;

/*
struct Global {
    int xres, yres;
    Grid grid[MAX_GRID][MAX_GRID];
    Snake snake;
    Rat rats [100];;
    int gridDim;
    int boardDim;
    int gameover;
    int winner;
    int score;
    int show_credits;
    Image *marbleImage;
    GLuint marbleTexture;
    Button button[MAXBUTTONS];
    int nbuttons;
    //
    ALuint alBufferDrip, alBufferTick;
    ALuint alSourceDrip, alSourceTick;

    Flt camera[2];

    Global() {
        xres = 800;
        yres = 600;
        gridDim = 40;
        gameover = 0;
        winner = 0;
        int score = 0;
        nbuttons = 0;
        marbleImage=NULL;
        camera[0] = camera[1] = 0.0;
        show_credits = 0;
    }
} g;
*/
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

/* in main code for now, doesnt work from here

void initRat()
{
    for (int i = 0; i<10; i++){
        rats[i].status = 1;
        rats[i].pos[0] = 30 - (i*3);
        rats[i].pos[0] = 10 +(i*2);
    }
    
}
*/

int toggleCredits(int credits)
{
    if(credits == 0)
        return 1;
    else 
        return 0;
}
