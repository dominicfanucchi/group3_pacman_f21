//Juan Sierra Diaz
//Fall 2021


#include <X11/Xlib.h>
#include <GL/glx.h>
#include"fonts.h"

void show_juan_credits(int x, int y){

        Rect r;

        r.bot = y;
        r.left = x;
        r.center = 1;

        ggprint8b(&r, 16, 0x00ff0000, "Juan Sierra Diaz");



}

//Working on adding a main display. 

struct Welcome_Display {
    int x, y;

    Welcome_Display() {
        x = 800;
        y = 800;


    }
}w;



void mainDisplay(void){
    glViewport(0, 0, w.x, w.y);
    //clear color buffer
    glClearColor(0.5f, 0.2f, 0.3f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    //init matrices
    glMatrixMode (GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //this sets to 2D mode (no perspective)
    glOrtho(0, w.x, 0, w.y, -1, 1);
    //screen background
    glColor3f(0.5f, 0.5f, 0.5f);
    //glBindTexture(GL_TEXTURE_2D, g.marbleTexture);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(0, 0);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(0, w.y);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(w.x, w.y);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(w.x, 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);



    Rect r;

    r.bot = 400;
    r.left = 400;
    r.center = 1;

    ggprint8b(&r, 16, 0x00ff0000, "Welcome to Pacman!");
    ggprint8b(&r, 16, 0x00ff0000, "Press p to Play!");
}


