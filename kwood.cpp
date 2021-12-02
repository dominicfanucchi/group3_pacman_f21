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
/*Kenneths work, translated snake to pacman and rats to pellets for
// our game

void initPacman()
{
    int i;
    g.pacman.status = 1;
    g.pacman.delay = .15;
    g.pacman.length = 1;

    for (i=0; i<g.pacman.length; i++) {
        g.pacman.pos[i][0] = 20;
        g.pacman.pos[i][1] = 42;
    }

    g.pacman.direction = DIRECTION_RIGHT;

}
Also Kenneth's work
void initPellets()
{

    for (int i = 0; i<10; i++){
        g.pellets[i].status = 1;
        g.pellets[i].pos[0] = 30 - (i*3);
        g.pellets[i].pos[1] = 10 +(i*2);
    }
    
    g.pellets[0].status = 1;
    g.[1].status = 1;
    g.rats[0].pos[0] = 25;
    g.rats[1].pos[0] = 12;
    g.rats[0].pos[1] = 2;
    g.rats[1].pos[1] = 23;
    
}

// in main code for now, doesnt work from here

void initRat()
{
    for (int i = 0; i<10; i++){
        rats[i].status = 1;
        rats[i].pos[0] = 30 - (i*3);
        rats[i].pos[0] = 10 +(i*2);
    }
    
}
*/


/*
void makePathVertical(int xPos, int yStart, int yEnd)
{

    int k = 0;
    while (k<100){
        g.vertPaths[k].pos[0][0] = xPos;
        g.vertPaths[k].pos[0][1] = 1+k;
      //  printf("\n%d,%d,%d", k ,g.walls[k].pos[0][0], g.walls[k].pos[0][1]);
        k++;
    }

}
void makePathHorizontal(int yPos, int xStart, int index)
{

    int c =index;
    while (c<xEnd){
    g.horPaths[c].pos[0][0] = xStart+c-index;
    g.horPaths[c].pos[0][1] = yPos;
    printf("\n%d ,%d,%d",c,  g.horPaths[c].pos[0][0], g.horPaths[c].pos[0][1]);
    c++;
       }
}
*/


// Very janky movement detection, would do this better if i had the chance to again.

/* 
         case DIRECTION_UP:
                                if(isValidVert(g.pacman.pos[0][0])){
                                  g.pacman.pos[0][1] -=1;
                                  g.a=-5;
                                  g.s=-5;
                                  g.d=-5;
                                  g.f=43;
                                  g.g=43;
                                  g.h=43;
                                  g.q=-5;
                                  g.w=43;
                                  g.e = -5;
                                  g.r = 43;
                                  vertWalls(g.pacman.pos[0][0]);
                                  if((g.pacman.pos[0][1] == g.a) || (g.pacman.pos[0][1] == g.s) || (g.pacman.pos[0][1] == g.d)
                                            || (g.pacman.pos[0][1] == g.f) ||(g.pacman.pos[0][1] == g.g) ||(g.pacman.pos[0][1] == g.h)
                                            ||  (g.pacman.pos[0][1] == g.q)|| (g.pacman.pos[0][1] == g.w) || (g.pacman.pos[0][1] == g.e)
                                            || (g.pacman.pos[0][1] == g.r))
                                            g.pacman.pos[0][1] +=1;
                              }
                              break;

        case DIRECTION_DOWN:
                              if(isValidHor(g.pacman.pos[0][1])){
                                  g.pacman.pos[0][0] +=1;
                                  g.a=-5;
                                  g.s=-5;
                                  g.d=43;
                                  g.f=43;
                                  g.g=-5;
                                  g.h=43;
                                  g.q=-5;
                                  g.w=43;
                                  horWalls(g.pacman.pos[0][1]);
                                if (g.pacman.pos[0][0] == 38 && g.pacman.pos[0][1] == 17)
                                    g.pacman.pos[0][0] = -3;
                                if((g.pacman.pos[0][0] == g.a) || (g.pacman.pos[0][0] == g.s) || (g.pacman.pos[0][0] == g.d)
                                             || (g.pacman.pos[0][0] == g.f) ||(g.pacman.pos[0][0] == g.g) ||(g.pacman.pos[0][0] == g.h)
                                             ||  (g.pacman.pos[0][0] == g.q)|| (g.pacman.pos[0][0] == g.w))
                                             g.pacman.pos[0][0] -=1;
                              }
                              break;
    }

        case DIRECTION_UP:
                                if(isValidVert(g.pacman.pos[0][0])){
                                  g.pacman.pos[0][1] -=1;
                                  g.a=-5;
                                  g.s=-5;
                                  g.d=-5;
                                  g.f=43;
                                  g.g=43;
                                  g.h=43;
                                  g.q=-5;
                                  g.w=43;
                                  g.e = -5;
                                  g.r = 43;
                                  vertWalls(g.pacman.pos[0][0]);
                                  if((g.pacman.pos[0][1] == g.a) || (g.pacman.pos[0][1] == g.s) || (g.pacman.pos[0][1] == g.d)
                                            || (g.pacman.pos[0][1] == g.f) ||(g.pacman.pos[0][1] == g.g) ||(g.pacman.pos[0][1] == g.h)
                                            ||  (g.pacman.pos[0][1] == g.q)|| (g.pacman.pos[0][1] == g.w) || (g.pacman.pos[0][1] == g.e)
                                            || (g.pacman.pos[0][1] == g.r))
                                            g.pacman.pos[0][1] +=1;
                              }
                              break;

        case DIRECTION_LEFT:
                              if(isValidHor(g.pacman.pos[0][1])){
                                  g.pacman.pos[0][0] +=1;
                                  g.a=-5;
                                  g.s=-5;
                                  g.d=43;
                                  g.f=43;
                                  g.g=-5;
                                  g.h=43;
                                  g.q=-5;
                                  g.w=43;
                                  horWalls(g.pacman.pos[0][1]);
                                if (g.pacman.pos[0][0] == 38 && g.pacman.pos[0][1] == 17)
                                    g.pacman.pos[0][0] = -3;
                                if((g.pacman.pos[0][0] == g.a) || (g.pacman.pos[0][0] == g.s) || (g.pacman.pos[0][0] == g.d)
                                             || (g.pacman.pos[0][0] == g.f) ||(g.pacman.pos[0][0] == g.g) ||(g.pacman.pos[0][0] == g.h)
                                             ||  (g.pacman.pos[0][0] == g.q)|| (g.pacman.pos[0][0] == g.w))
                                             g.pacman.pos[0][0] -=1;
                              }
                              break;
    }
*/

int toggleCredits(int credits)
{
    if(credits == 0)
        return 1;
    else 
        return 0;
}
