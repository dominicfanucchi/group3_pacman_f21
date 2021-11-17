//Dominic Fanucchi
//program: dfanucchi.cpp
//author: Dominic Fanucchi
//date: September 2021

#include "fonts.h"
#include <iostream>
//#include "dfanucchi.h"
using namespace std;

unsigned int maze_tex;

void show_dominics_credits(int x, int y)
{
    Rect r;
    r.bot = y;
    r.left = x;
    r.center = 5;
    ggprint8b(&r, 16, 0x00ffff34, "Dominic Fanucchi");
}

// void load_and_bind_textures()
// {
// 	glEnable(GL_BLEND);
//     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

// 	maze_tex = load_and_bind_texture("images/maze.png");
// }

// unsigned int load_and_bind_texture(const char* filename)
// {
	
// }

// typedef enum {#, g, u} tile;
// // Tile defined as an enum where each type is defined as:
// // #: Wall
// // g: Ghost Pen Gate
// // u: Empty path
// tile maze[28][31] =
// {
//     {#,#,#,#,#,#,#,#,#,#,#,#,u,u,u,#,u,#,u,u,u,#,#,#,#,#,#,#,#,#,#},
//     {#,u,u,u,u,#,#,u,u,u,u,#,u,u,u,#,u,#,u,u,u,#,u,u,u,u,u,u,u,u,#},
//     {#,u,#,#,u,#,#,u,#,#,u,#,u,u,u,#,u,#,u,u,u,#,u,#,#,u,#,#,#,u,#},
//     {#,u,#,#,u,u,u,u,#,#,u,#,u,u,u,#,u,#,u,u,u,#,u,#,#,u,#,#,#,u,#},
//     {#,u,#,#,u,#,#,#,#,#,u,#,u,u,u,#,u,#,u,u,u,#,u,#,#,u,#,#,#,u,#},
//     {#,u,#,#,u,#,#,#,#,#,u,#,#,#,#,#,u,#,#,#,#,#,u,#,#,u,#,#,#,u,#},
//     {#,u,#,#,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,#},
//     {#,u,#,#,#,#,#,u,#,#,u,#,#,#,#,#,u,#,#,#,#,#,#,#,#,u,#,#,#,u,#},
//     {#,u,#,#,#,#,#,u,#,#,u,#,#,#,#,#,u,#,#,#,#,#,#,#,#,u,#,#,#,u,#},
//     {#,u,#,#,u,u,u,u,#,#,u,u,u,u,u,u,u,u,u,u,#,#,u,u,u,u,#,#,#,u,#},
//     {#,u,#,#,u,#,#,u,#,#,u,#,#,u,#,#,#,#,#,u,#,#,u,#,#,u,#,#,#,u,#},
//     {#,u,#,#,u,#,#,u,#,#,u,#,#,u,#,u,u,u,#,u,#,#,u,#,#,u,#,#,#,u,#},
//     {#,u,u,u,u,#,#,u,u,u,u,#,#,u,#,u,u,u,#,u,u,u,u,#,#,u,u,u,u,u,#},
//     {#,u,#,#,#,#,#,u,#,#,#,#,#,u,#,u,u,u,g,u,#,#,#,#,#,u,#,#,#,#,#},
//     {#,u,#,#,#,#,#,u,#,#,#,#,#,u,#,u,u,u,g,u,#,#,#,#,#,u,#,#,#,#,#},
//     {#,u,u,u,u,#,#,u,u,u,u,#,#,u,#,u,u,u,#,u,u,u,u,#,#,u,u,u,u,u,#},
//     {#,u,#,#,u,#,#,u,#,#,u,#,#,u,#,u,u,u,#,u,#,#,u,#,#,u,#,#,#,u,#},
//     {#,u,#,#,u,#,#,u,#,#,u,#,#,u,#,#,#,#,#,u,#,#,u,#,#,u,#,#,#,u,#},
//     {#,u,#,#,u,u,u,u,#,#,u,u,u,u,u,u,u,u,u,u,#,#,u,u,u,u,#,#,#,u,#},
//     {#,u,#,#,#,#,#,u,#,#,u,#,#,#,#,#,u,#,#,#,#,#,#,#,#,u,#,#,#,u,#},
//     {#,u,#,#,#,#,#,u,#,#,u,#,#,#,#,#,u,#,#,#,#,#,#,#,#,u,#,#,#,u,#},
//     {#,u,#,#,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,#},
//     {#,u,#,#,u,#,#,#,#,#,u,#,#,#,#,#,u,#,#,#,#,#,u,#,#,u,#,#,#,u,#},
//     {#,u,#,#,u,#,#,#,#,#,u,#,u,u,u,#,u,#,u,u,u,#,u,#,#,u,#,#,#,u,#},
//     {#,u,#,#,u,u,u,u,#,#,u,#,u,u,u,#,u,#,u,u,u,#,u,#,#,u,#,#,#,u,#},
//     {#,u,#,#,u,#,#,u,#,#,u,#,u,u,u,#,u,#,u,u,u,#,u,#,#,u,#,#,#,u,#},
//     {#,u,u,u,u,#,#,u,u,u,u,#,u,u,u,#,u,#,u,u,u,#,u,u,u,u,u,u,u,u,#},
//     {#,#,#,#,#,#,#,#,#,#,#,#,u,u,u,#,u,#,u,u,u,#,#,#,#,#,#,#,#,#,#}
// };

// void translate_to_maze_coords(float x, float y)
// {
//     glTranslatef(x * 8, y * 8, 0.0f);
// }

// void translate_bottom_left()
// {
//     glTranslatef(38.0f, 26.0f, 0.0f);
// }

// tile get_tile(float x, float y)
// {
//     return maze[(int) floor(x)][(int) floor(y)];
// }

// void drawMaze()
// {
//     glPushMatrix();
//     translate_bottom_left();              // Translate to bottom left of screen
//     draw_texture(maze_tex, 224, 248, 0);     // Draw the map walls using the sprite
//     glPopMatrix();
// }

// void draw_texture(unsigned int texture, int length, int height, float angle)
// {
//     // Begin new transformation matrix
//     glPushMatrix();
//     glColor3f(1.0f, 1.0f, 1.0f);

//     int widthCenter = length/2;
//     int verticalCenter = height/2;

//     // Translate to center of sprite to rotate about its origin (for sprites such as pacman)
//     glTranslatef((float)widthCenter,(float)verticalCenter,0.0f);
//     glRotatef(angle, 0.0f, 0.0f, 1.0f);

//     // Enable texturing and bind the specific sprite
//     glEnable(GL_TEXTURE_2D);
//     glBindTexture(GL_TEXTURE_2D, texture);

//     // texture coordinates
//     glBegin(GL_QUADS);
//         glTexCoord2f (0.0f, 0.0f);      // Lower left corner
//         glTexCoord2f (1.0f, 0.0f);      // Lower right corner
//         glTexCoord2f (1.0f, 1.0f);      // Upper right corner
//         glTexCoord2f (0.0f, 1.0f);      // Upper left corner

//         glVertex2i(-widthCenter, -verticalCenter);
//         glVertex2i( widthCenter, -verticalCenter);
//         glVertex2i( widthCenter,  verticalCenter);
//         glVertex2i(-widthCenter,  verticalCenter);
//     glEnd();

//     glDisable(GL_TEXTURE_2D);

//     // Pop matrix to ignore above transformations on future objects
//     glPopMatrix();
// }