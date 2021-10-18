// Kenneth Wood
//Kenneth Wood's c++ personal file for 3350 project

#include "fonts.h"

void show_kenneth_credits(int x, int y)
{
    Rect r;
    r.bot = y;
    r.left = x;
    r.center =1;
    ggprint8b(&r, 16, 0x00ffff34, "Kenneths work");
}
