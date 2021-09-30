//Dominic Fanucchi
//program: dfanucchi.cpp
//author: Dominic Fanucchi
//date: September 2021


#include "fonts.h"
#include <iostream>
#include "dfanucchi.h"
using namespace std;

void show_dominics_credits(int x, int y)
{
    Rect r;

    r.bot = y;
    r.left = x;
    r.center = 1;

    ggprint8b(&r, 16, 0x00ff0000, "Dominic Fanucchi");
}
