//Juan Sierra Diaz

#include"fonts.h"

void show_juan_credits(int x, int y){

        Rect r;

        r.bot = y;
        r.left = x;
        r.center = 0;

        ggprint8b(&r, 16, 0x00A020F0, "Juan Sierra Diaz");



}

