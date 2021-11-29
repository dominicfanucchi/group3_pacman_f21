//Juan Sierra Diaz
//Fall 2021


#include <X11/Xlib.h>
#include <GL/glx.h>
#include"fonts.h"


#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>


class Image {
public:


        int width, height;
        unsigned char *data;
        ~Image() { delete [] data; }
        Image(const char *fname) {

                if (fname[0] == '\0')
                        return;
                //printf("fname **%s**\n", fname);
                int ppmFlag = 0;
                char name[40];
                strcpy(name, fname);
                int slen = strlen(name);
                char ppmname[80];
                if (strncmp(name+(slen-4), ".ppm", 4) == 0)
                        ppmFlag = 1;
                if (ppmFlag) {
                        strcpy(ppmname, name);
                } else {
                        name[slen-4] = '\0';
                        //printf("name **%s**\n", name);
                        sprintf(ppmname,"%s.ppm", name);
                        //printf("ppmname **%s**\n", ppmname);
                        char ts[100];
                        //system("convert img.jpg img.ppm");
                        sprintf(ts, "convert %s %s", fname, ppmname);
                        system(ts);
                }

                //sprintf(ts, "%s", name);
                FILE *fpi = fopen(ppmname, "r");
                if (fpi) {
                        char line[200];
                        fgets(line, 200, fpi);
                        fgets(line, 200, fpi);
                        //skip comments and blank lines
                        while (line[0] == '#' || strlen(line) < 2)
                                fgets(line, 200, fpi);
                        sscanf(line, "%i %i", &width, &height);
                        fgets(line, 200, fpi);
                        //get pixel data
                        int n = width * height * 3;
                        data = new unsigned char[n];
                        for (int i=0; i<n; i++)
                                data[i] = fgetc(fpi);
                        fclose(fpi);
                } else {
                        printf("ERROR opening image: %s\n",ppmname);
                        exit(0);
                }
                if (!ppmFlag)
                        unlink(ppmname);
        }
};
Image img[1] = {"./images/main_display.jpg" };



struct Welcome_Display {
    int x, y;
	
    GLuint mainTexture;
    Image *mainDisplay;


    Welcome_Display() {
        x = 815;
        y = 800;
	mainDisplay=NULL;


    }
}w;

void initOpengl2(void)
{
        //OpenGL initialization
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_COLOR_MATERIAL);
        //
        //choose one of these
        //glShadeModel(GL_FLAT);
        glShadeModel(GL_SMOOTH);
        glDisable(GL_LIGHTING);
        glBindTexture(GL_TEXTURE_2D, 0);
        //
        glEnable(GL_TEXTURE_2D);
        //
        glBindTexture(GL_TEXTURE_2D, 0);
        //
        //load the image file into a ppm structure.
        //
        //
        w.mainDisplay = &img[0];
        //
        //create opengl texture elements
        glGenTextures(1, &w.mainTexture);
        glBindTexture(GL_TEXTURE_2D, w.mainTexture);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, 3,
                     w.mainDisplay->width, w.mainDisplay->height,
                     0, GL_RGB, GL_UNSIGNED_BYTE, w.mainDisplay->data);
}




void show_juan_credits(int x, int y){

        Rect r;

        r.bot = y;
        r.left = x;
        r.center = 1;

        ggprint8b(&r, 16, 0x00ff0000, "Juan Sierra Diaz");



}

//Working on adding a main display. 

void mainDisplay(void){

    initOpengl2();
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
    glBindTexture(GL_TEXTURE_2D, w.mainTexture);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(0, w.y);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(w.x, w.y);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(w.x, 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);



    Rect r;

    r.bot = 200;
    r.left = 400;
    r.center = 1;

    ggprint8b(&r, 16, 0x00ff0000, "Welcome to Pacman!");
    ggprint8b(&r, 16, 0x00ff0000, "Press p to Play!");
}


