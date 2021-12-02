#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include "fonts.h"
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
//#include "ppm.h"
//#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>

#define USE_OPENAL_SOUND
#ifdef USE_OPENAL_SOUND
#include </usr/include/AL/alut.h>
#endif //USE_OPENAL_SOUND

//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt     Matrix[4][4];
int main_game = 0;
using namespace std;

//macros
#define rnd() (double)rand()/(double)RAND_MAX
#define DIRECTION_DOWN  0
#define DIRECTION_LEFT  1
#define DIRECTION_UP    2
#define DIRECTION_RIGHT 3
#define MAX_GRID 80
typedef struct t_grid {
    int status;
    float color[4];
} Grid;

typedef struct t_wall {
    int status;
    int pos[MAX_GRID*MAX_GRID][2];
} Wall;

typedef struct t_pacman {
    int status;
    int length;
    int pos[MAX_GRID*MAX_GRID][2];
    int direction;
    double timer;
    double delay;
} Pac;

typedef struct t_pellet {
    int status;
    int pos[2];
} Pellet;

#define MAXBUTTONS 4
typedef struct t_button {
    Rect r;
    char text[32];
    int over;
    int down;
    int click;
    float color[3];
    float dcolor[3];
    unsigned int text_color;
} Button;

class Level {
public:
    unsigned char arr[64][90];
    int nrows, ncols;
    int tilesize[1];
    Flt ftsz[2];
    Flt tile_base;
    Level() {
        //Log("Level constructor\n");
        tilesize[0] = 5;
        tilesize[1] = 10;
        ftsz[0] = (Flt)tilesize[0];
        ftsz[1] = (Flt)tilesize[1];
        tile_base = 100;
        //read level

        FILE *fpi = fopen("map2.txt","r");
        if (fpi) {
            nrows=0;
            char line[100];
            while (fgets(line, 100, fpi) != NULL) {
                removeCrLf(line);
                int slen = strlen(line);
                ncols = slen;
                //Log("line: %s\n", line);
                for (int j=0; j<slen; j++) {
                    arr[nrows][j] = line[j];
                }
                ++nrows;
            }
            fclose(fpi);
            //printf("nrows of background data: %i\n", nrows);
        }

        for (int i=0; i<nrows; i++) {
            for (int j=0; j<ncols; j++) {
                printf("%c", arr[i][j]);
            }
            printf("\n");
        }
    }
    void removeCrLf(char *str) { //remove carriage return and linefeed from a Cstring
        char *p = str;
        while (*p) {
            if (*p == 10 || *p == 13) {
                *p = '\0';
                break;
            }
        ++p;
        }
    }
} lev;

// class Image {
// public:
//     int width, height;
//     unsigned char *data;
//     ~Image() { delete [] data; }
//     Image(const char *fname) {
//         if (fname[0] == '\0')
//             return;
//         //printf("fname **%s**\n", fname);
//         int ppmFlag = 0;
//         char name[40];
//         strcpy(name, fname);
//         int slen = strlen(name);
//         char ppmname[80];
//         if (strncmp(name+(slen-4), ".ppm", 4) == 0)
//             ppmFlag = 1;
//         if (ppmFlag) {
//             strcpy(ppmname, name);
//         } else {
//             name[slen-4] = '\0';
//             //printf("name **%s**\n", name);
//             sprintf(ppmname,"%s.ppm", name);
//             //printf("ppmname **%s**\n", ppmname);
//             char ts[100];
//             //system("convert img.jpg img.ppm");
//             sprintf(ts, "convert %s %s", fname, ppmname);
//             system(ts);
//         }
//         //sprintf(ts, "%s", name);
//         FILE *fpi = fopen(ppmname, "r");
//         if (fpi) {
//             char line[200];
//             fgets(line, 200, fpi);
//             fgets(line, 200, fpi);
//             //skip comments and blank lines
//             while (line[0] == '#' || strlen(line) < 2)
//                 fgets(line, 200, fpi);
//             sscanf(line, "%i %i", &width, &height);
//             fgets(line, 200, fpi);
//             //get pixel data
//             int n = width * height * 3;         
//             data = new unsigned char[n];            
//             for (int i=0; i<n; i++)
//                 data[i] = fgetc(fpi);
//             fclose(fpi);
//         } else {
//             printf("ERROR opening image: %s\n",ppmname);
//             exit(0);
//         }
//         if (!ppmFlag)
//             unlink(ppmname);
//     }
// };
// Image img[1] = {"./images/" };

struct Global {
    int xres, yres;
    Grid grid[MAX_GRID][MAX_GRID];
    int gridDim;
    Pac pacman;
    Pellet pellets[2020];
    Pellet bigPellets[4];
    Wall vertPaths[100];
    Wall horPaths[100];
    int a,s,d,f,g,h,q,w,e,r,t,y;
    int boardDim;
    int gameover;
    int winner;
    int score;
    int show_credits;
    //Image *marbleImage;
    //GLuint marbleTexture;
    Button button[MAXBUTTONS];
    int nbuttons;
    
    Flt camera[2];  

    Global() {
        xres = 815;
        yres = 800;
        gridDim = 40;
        gameover = 0;
        winner = 0;
        nbuttons = 0;
        camera[0] = camera[1] = 0.0;
        show_credits = 0;
    }
} g;

class X11_wrapper {
private:
    Display *dpy;
    Window win;
public:
    X11_wrapper() {
        GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
        //GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
        XSetWindowAttributes swa;
        setupScreenRes(g.xres, g.yres);
        dpy = XOpenDisplay(NULL);
        if (dpy == NULL) {
            printf("\n\tcannot connect to X server\n\n");
            exit(EXIT_FAILURE);
        }
        Window root = DefaultRootWindow(dpy);
        XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
        if (vi == NULL) {
            printf("\n\tno appropriate visual found\n\n");
            exit(EXIT_FAILURE);
        } 
        Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
        swa.colormap = cmap;
        swa.event_mask = ExposureMask |
                            KeyPressMask |
                            KeyReleaseMask |
                            ButtonPressMask |
                            ButtonReleaseMask |
                            PointerMotionMask |
                            StructureNotifyMask |
                            SubstructureNotifyMask;
        win = XCreateWindow(dpy, root, 0, 0, g.xres, g.yres, 0, vi->depth,
                        InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
        GLXContext glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
        glXMakeCurrent(dpy, win, glc);
        setTitle();
    }
    ~X11_wrapper() {
        XDestroyWindow(dpy, win);
        XCloseDisplay(dpy);
    }
    void setTitle() {
        //Set the window title bar.
        XMapWindow(dpy, win);
        XStoreName(dpy, win, "Pakkuman");
    }
    void setupScreenRes(const int w, const int h) {
        g.xres = w;
        g.yres = h;
    }
    void reshapeWindow(int width, int height) {
        //window has been resized.
        setupScreenRes(width, height);
        //
        glViewport(0, 0, (GLint)width, (GLint)height);
        glMatrixMode(GL_PROJECTION); glLoadIdentity();
        glMatrixMode(GL_MODELVIEW); glLoadIdentity();
        glOrtho(0, g.xres, 0, g.yres, -1, 1);
        setTitle();
    }
    void checkResize(XEvent *e) {
        //The ConfigureNotify is sent by the server if the window is resized.
        if (e->type != ConfigureNotify)
            return;
        XConfigureEvent xce = e->xconfigure;
        if (xce.width != g.xres || xce.height != g.yres) {
            //Window size did change.
            reshapeWindow(xce.width, xce.height);
        }
    }
    bool getXPending() {
        return XPending(dpy);
    }
    XEvent getXNextEvent() {
        XEvent e;
        XNextEvent(dpy, &e);
        return e;
    }
    void swapBuffers() {
        glXSwapBuffers(dpy, win);
    }
} x11;

//function prototypes
void initOpengl(void);
int checkMouse(XEvent *e);
int checkKeys(XEvent *e);
int isValidHor(int x);
int toggleCredits(int credits);
void init();
extern void initSounds(void);
void physics(void);
extern void mainDisplay(void);
void render(void);
void getGridCenter(const int i, const int j, int cent[2]);
#ifdef USE_OPENAL_SOUND
extern void initSound();
extern void cleanupSound();
extern void playSound(ALuint source);
#endif //USE_OPENAL_SOUND
int quitGame();
void drawPellets(int,int,int,int);
void deletePellets();

//Setup timers
const double physicsRate = 1.0 / 60.0;
const double oobillion = 1.0 / 1e9;
struct timespec timeStart, timeCurrent;
struct timespec timePause;
double physicsCountdown = 0.0;
double timeSpan = 0.0;
double timeDiff(struct timespec *start, struct timespec *end) {
    return (double)(end->tv_sec - start->tv_sec ) +
            (double)(end->tv_nsec - start->tv_nsec) * oobillion;
}
void timeCopy(struct timespec *dest, struct timespec *source) {
    memcpy(dest, source, sizeof(struct timespec));
}

int main(int argc, char *argv[])
{
    if (argc) {}
    if (argv[0]) {}
    logOpen();
    initOpengl();
    init();
    initialize_fonts();
    initSound();
    srand((unsigned int)time(NULL));
    clock_gettime(CLOCK_REALTIME, &timePause);
    clock_gettime(CLOCK_REALTIME, &timeStart);
    int done = 0;
    mainDisplay();
    while (!done) {
        while (x11.getXPending()) {
            XEvent e = x11.getXNextEvent();
            x11.checkResize(&e);
            done = checkMouse(&e);
            done = checkKeys(&e);
        }
        //
        //Below is a process to apply physics at a consistent rate.
        //1. Get the time right now.
        clock_gettime(CLOCK_REALTIME, &timeCurrent);
        //2. How long since we were here last?
        timeSpan = timeDiff(&timeStart, &timeCurrent);
        //3. Save the current time as our new starting time.
        timeCopy(&timeStart, &timeCurrent);
        //4. Add time-span to our countdown amount.
        physicsCountdown += timeSpan;
        //5. Has countdown gone beyond our physics rate? 
        //       if yes,
        //           In a loop...
        //              Apply physics
        //              Reducing countdown by physics-rate.
        //              Break when countdown < physics-rate.
        //       if no,
        //           Apply no physics this frame.
        while(physicsCountdown >= physicsRate) {
            //6. Apply physics
            physics();
            //7. Reduce the countdown by our physics-rate
            physicsCountdown -= physicsRate;
        }
	mainDisplay();
        //Always render every frame.
	if(main_game){

        	render();
	}
        x11.swapBuffers();
    }
    cleanupSound();
    cleanup_fonts();
    logClose();
    return 0;
}

void initOpengl(void)
{
    //OpenGL initialization
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_COLOR_MATERIAL);
    
    //choose one of these
    //glShadeModel(GL_FLAT);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_LIGHTING);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glEnable(GL_TEXTURE_2D);
    //marble_texture = loadBMP("marble.bmp");
    glBindTexture(GL_TEXTURE_2D, 0);
    
    //load the image file into a ppm structure.
    
    //g.marbleImage = ppm6GetImage("./images/marble.ppm");
    //g.marbleImage = &img[0];
   
    //create opengl texture elements
    //glGenTextures(1, &g.marbleTexture);
    //glBindTexture(GL_TEXTURE_2D, g.marbleTexture);
    // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    // glTexImage2D(GL_TEXTURE_2D, 0, 3,
    //              g.marbleImage->width, g.marbleImage->height,
    //              0, GL_RGB, GL_UNSIGNED_BYTE, g.marbleImage->data);
}

/*Kenneth's work: Makes a horizontal path that pacman and ghosts
//can travel on. Ended up not being used but was how i started the paths.

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

void makePathHorizontal(int yPos, int xStart, int xEnd, int index)
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


// Kenneths work, translated snake to pacman and rats to pellets for 
// our game

void initPacman()
{
    int i;
    g.pacman.status = 1;
    g.pacman.delay = .15;
    g.pacman.length = 1;
    
    for (i=0; i<g.pacman.length; i++) {
        g.pacman.pos[i][0] = 2;
        g.pacman.pos[i][1] = 2;
    }
    
    g.pacman.direction = DIRECTION_RIGHT;

}
// Also Kenneth's work
// Juan Also contributed to this function
//
void drawPellets(int a, int b, int p,int d)
{

        for(int i = a; i < b; i++){

                g.pellets[i].status = 1;
                g.pellets[i].pos[0] = p;
                g.pellets[i].pos[1] =d+i;

        }
}

void drawBigPellets(int a, int p, int d) {
        
        g.bigPellets[0+a].status =1;
        g.bigPellets[0+a].pos[0] = p;
        g.bigPellets[0+a].pos[1] = d;
    
}
void initPellets()
{     
        int count = -2;
        int update = 0;
        int column = 48;
        int x = -4;
        do{
                drawPellets(update,column,count,x);
                update = update + 48;
                column = column + 48;
                x = x - 48;
                count++;
        }
        while(count <= 39);
	    deletePellets();

       drawBigPellets(0, -1, -4);
       drawBigPellets(1, 39, -4);
       drawBigPellets(2, -1, 39);
       drawBigPellets(3, 39, 39);
       drawBigPellets(4, 19, 32);
}


// Juan'work
// This funcition would be use
// to delete the unnecessary pellets in the paths
// however it will take a lot of time given the
// nature of how we pollulated the the pellets
// in the map. We could possibly also find a more
// efficent way to do it faster.
void deletePellets() {
    for(int i=0; i<48; i++) {
        g.pellets[i].status = 0;
    }
    int x_pos = 0;
    int pellet_deleted = 737;
    int update_pellet = 48;
    do {
    	for(int i=0; i<9; i++) {
    		g.pellets[pellet_deleted+i].status = 0;
        }
        x_pos++;
        pellet_deleted = update_pellet + pellet_deleted;
<<<<<<< HEAD
    } while(x_pos <= 12);
=======
    } while(x_pos <= 13);
>>>>>>> 391608e4d86c636247de7522013d358f4baac063
}



int isValidHor(int y)
{
        switch(y){
case -4:
case 2:
case 22:
case 12:
case 37:
case 7:
case 17:
case 27:
case 32:
case 42:
      return true;
        }      
      return false;

}
 int isValidVert(int x)
{
        switch(x){
case 7:
case -1:
case 12:
case 22:
case 21:
case 39:
case 2:
case 37:
case 17:
case 27:
case 32:
      return true;
        }      
      return false;

}
void horWalls(int y)
{

    switch (y){
        case -4:
            g.a = 18;
            g.s = 20;
            break;
        case 2:
            g.a = -2;
            g.s = 40;
            break;
        case 7:
            g.a = -2;
            g.s = 8;
            g.d = 18;
            g.f = 19;
            g.g = 28;
            g.h = 31;
            g.q = 40;
            g.w = 10;
            break;
        case 12:
            g.a =-2;
            g.s = 6;
            g.d = 8;
            g.f = 28;
            g.g = 31;
            g.h= 33;
            g.q = 11;
            break;
        case 17:
            g.a = 26;
            g.s = 13;
            break;
        case 22:
            g.a = 6;
            g.s = 8;
            g.d = 11;
            g.f = 28;
            g.g = 31;
            g.h = 33;
            break;
        case 27: 
            g.a = -2;
            g.s = 18; 
            g.d = 20;
            g.f = 40;
            break;
        case 32:
            g.a = -2;
            g.s = 3;
            g.d = 6;
            g.f = 33;
            g.g = 36;
            g.h = 40;
            break;
        case 37: 
            g.a = -2;
            g.s = 28;
            g.d = 30;
            g.f = 40;
            g.g = 19;
            g.h = 17;
            g.q = 8;
            g.w = 10;
            break;
        case 42:
            g.a = -2;
            g.s = 40;
            break;
    
 }
}
void vertWalls(int x){
    switch(x){
        case 7:
            g.a =-5;
            g.s = 38;
            g.d = 43;
            break;
        case 32:
            g.a =-5;
            g.s = 38;
            g.d = 43;
            break;
        case -1:
            g.a = 43;
            g.s = 36;
            g.d = 33;
            g.f = 26;
            g.g = 18;
            g.h = 16;
            g.q = -5;
            break;
        case 39: 
            g.a = 43;
            g.s = 36;
            g.d = 33;
            g.f = 26;
            g.g = 18;
            g.h = 16;
            g.q = -5;
            break;
        case 12:
            g.a = 8;
            g.s = 1;
            g.d = -3;
            g.f = -5;
            g.g = 11;
            g.h = 29;
            g.q = 31;
            g.w = 39;
            g.e = 41;
            g.r = 43;
            break;
        case 27:
            g.a = 8;
            g.s = 1;
            g.d = -3;
            g.f = -5;
            g.g = 11;
            g.h = 29;
            g.q = 31;
            g.w = 39;
            g.e = 41;
            g.r = 43;
            break;
        case 21: 
            g.a = 36;
            g.s = 33;
            g.d = 26;
            g.f = 23;
            g.g = 21;
            g.h = 13;
            g.q = 11;
            g.w = 4;
            g.e = 2;
            break;
        case 17: 
            g.a = 36;
            g.s = 33;
            g.d = 26;
            g.f = 23;
            g.g = 21;
            g.h = 13;
            g.w = 4;
            g.e = 2;
            break;
        case 37: 
            g.a = 31;
            g.s = 28;
            g.d = 38;
            g.f = 41;
            g.g = 43;
            g.h = -5;
            break;
        case 2: 
            g.a = 31;
            g.s = 28;
            g.d = 38;
            g.f = 41;
            g.g = 43;
            g.h = -5;
            break;
 }

  
  /* 
    int l = 101;
   while (l<200){
    g.walls[l].pos[0][0] = 7;
    g.walls[l].pos[0][1] = 0+(l-100);
    printf("\n %d, %d, %d", l, g.walls[l].pos[0][0], g.walls[l].pos[0][1]);
    l++;
   }
    */
   
   /*
    g.walls[100].pos[0][0] = 7;
    g.walls[100].pos[0][1] = 2;
    g.walls[101].pos[0][0] = 7;
    g.walls[101].pos[0][1] = 3;
    g.walls[102].pos[0][0] = 7;
    g.walls[102].pos[0][1] = 1;
   */



    /*
    for (int i=0; i<4; i++)
    {
        for (int j=1; j<4; j++)
        {
        int count = j+(4*i);
        g.walls[count-1].pos[0][0] = 1;
        g.walls[count-1].pos[0][1] = 0-j;
        count++;
        printf("\n%d,%d",  g.walls[i].pos[0][0], g.walls[i].pos[0][1]);

    }
    } 

    */
}
void init()
{
    g.boardDim = g.gridDim * 10.0;
    //initWalls();
    initPacman();
    initPellets();
   
   
    //initialize buttons...
    g.nbuttons=0;
    //size and position
    g.button[g.nbuttons].r.width = 140;
    g.button[g.nbuttons].r.height = 60;
    g.button[g.nbuttons].r.left = 20;
    g.button[g.nbuttons].r.bot = 400;
    g.button[g.nbuttons].r.right =
       g.button[g.nbuttons].r.left + g.button[g.nbuttons].r.width;
    g.button[g.nbuttons].r.top =
       g.button[g.nbuttons].r.bot + g.button[g.nbuttons].r.height;
    g.button[g.nbuttons].r.centerx =
       (g.button[g.nbuttons].r.left + g.button[g.nbuttons].r.right) / 2;
    g.button[g.nbuttons].r.centery =
       (g.button[g.nbuttons].r.bot + g.button[g.nbuttons].r.top) / 2;
    strcpy(g.button[g.nbuttons].text, "Reset");
    g.button[g.nbuttons].down = 0;
    g.button[g.nbuttons].click = 0;
    g.button[g.nbuttons].color[0] = 0.4f;
    g.button[g.nbuttons].color[1] = 0.4f;
    g.button[g.nbuttons].color[2] = 0.7f;
    g.button[g.nbuttons].dcolor[0] = g.button[g.nbuttons].color[0] * 0.5f;
    g.button[g.nbuttons].dcolor[1] = g.button[g.nbuttons].color[1] * 0.5f;
    g.button[g.nbuttons].dcolor[2] = g.button[g.nbuttons].color[2] * 0.5f;
    g.button[g.nbuttons].text_color = 0x00ffffff;
    g.nbuttons++;
    g.button[g.nbuttons].r.width = 140;
    g.button[g.nbuttons].r.height = 60;
    g.button[g.nbuttons].r.left = 20;
    g.button[g.nbuttons].r.bot = 160;
    g.button[g.nbuttons].r.right =
       g.button[g.nbuttons].r.left + g.button[g.nbuttons].r.width;
    g.button[g.nbuttons].r.top = g.button[g.nbuttons].r.bot +
       g.button[g.nbuttons].r.height;
    g.button[g.nbuttons].r.centerx = (g.button[g.nbuttons].r.left +
       g.button[g.nbuttons].r.right) / 2;
    g.button[g.nbuttons].r.centery = (g.button[g.nbuttons].r.bot +
       g.button[g.nbuttons].r.top) / 2;
    strcpy(g.button[g.nbuttons].text, "Quit");
    g.button[g.nbuttons].down = 0;
    g.button[g.nbuttons].click = 0;
    g.button[g.nbuttons].color[0] = 0.3f;
    g.button[g.nbuttons].color[1] = 0.3f;
    g.button[g.nbuttons].color[2] = 0.6f;
    g.button[g.nbuttons].dcolor[0] = g.button[g.nbuttons].color[0] * 0.5f;
    g.button[g.nbuttons].dcolor[1] = g.button[g.nbuttons].color[1] * 0.5f;
    g.button[g.nbuttons].dcolor[2] = g.button[g.nbuttons].color[2] * 0.5f;
    g.button[g.nbuttons].text_color = 0x00ffffff;
    g.nbuttons++;
    g.button[g.nbuttons].r.width = 140;
    g.button[g.nbuttons].r.height = 60;
    g.button[g.nbuttons].r.left = 20;
    g.button[g.nbuttons].r.bot = 80;
    g.button[g.nbuttons].r.right =
       g.button[g.nbuttons].r.left + g.button[g.nbuttons].r.width;
    g.button[g.nbuttons].r.top = g.button[g.nbuttons].r.bot +
       g.button[g.nbuttons].r.height;
    g.button[g.nbuttons].r.centerx = (g.button[g.nbuttons].r.left +
       g.button[g.nbuttons].r.right) / 2;
    g.button[g.nbuttons].r.centery = (g.button[g.nbuttons].r.bot +
       g.button[g.nbuttons].r.top) / 2;
    strcpy(g.button[g.nbuttons].text, "Credits");
    g.button[g.nbuttons].down = 0;
    g.button[g.nbuttons].click = 0;
    g.button[g.nbuttons].color[0] = 0.3f;
    g.button[g.nbuttons].color[1] = 0.3f;
    g.button[g.nbuttons].color[2] = 0.6f;
    g.button[g.nbuttons].dcolor[0] = g.button[g.nbuttons].color[0] * 0.5f;
    g.button[g.nbuttons].dcolor[1] = g.button[g.nbuttons].color[1] * 0.5f;
    g.button[g.nbuttons].dcolor[2] = g.button[g.nbuttons].color[2] * 0.5f;
    g.button[g.nbuttons].text_color = 0x00ffffff;
    g.nbuttons++;
}

void resetGame()
{
    initPacman();
    initPellets();
    g.score = 0;
    g.gameover = 0;
    g.winner = 0;
}

int checkKeys(XEvent *e)
{
    static int shift=0;
    if (e->type != KeyRelease && e->type != KeyPress)
        return 0;
    int key = (XLookupKeysym(&e->xkey, 0) & 0x0000ffff);
    if (e->type == KeyRelease) {
        if (key == XK_Shift_L || key == XK_Shift_R)
            shift=0;
        return 0;
    }
    if (key == XK_Shift_L || key == XK_Shift_R) {
        shift=1;
        return 0;
    }
    (void)shift;
    switch (key) {
        case XK_r:
            resetGame();
            break;
        case XK_equal:
            g.pacman.delay *= 0.9;
            if (g.pacman.delay < 0.001)
                g.pacman.delay = 0.001;

            break;
        case XK_minus:
           g.pacman.delay *= (1.0 / 0.9);
            break;

        case XK_Left:
            g.pacman.direction = DIRECTION_LEFT;
            break;
        case XK_Right:
            g.pacman.direction = DIRECTION_RIGHT;
            break;
        case XK_Up:
            g.pacman.direction = DIRECTION_UP;
            break;
        case XK_Down:
            g.pacman.direction = DIRECTION_DOWN;
            break;

          
        case XK_c:
            g.show_credits = 1;
	    break;
	case XK_p:
	    main_game = 1;
	    break;
    }
    return 0;
}

int checkMouse(XEvent *e)
{
    static int savex = 0;
    static int savey = 0;
    int i,x,y;
    int lbutton=0;
    int rbutton=0;
    //
    if (e->type == ButtonRelease)
        return 0;
    if (e->type == ButtonPress) {
        if (e->xbutton.button==1) {
            //Left button is down
            lbutton=1;
        }
        if (e->xbutton.button==3) {
            //Right button is down
            rbutton=1;
            if (rbutton){}
        }
    }
    x = e->xbutton.x;
    y = e->xbutton.y;
    y = g.yres - y;
    if (savex != e->xbutton.x || savey != e->xbutton.y) {
        //Mouse moved
        savex = e->xbutton.x;
        savey = e->xbutton.y;
    }
    for (i=0; i<g.nbuttons; i++) {
        g.button[i].over=0;
        if (x >= g.button[i].r.left &&
            x <= g.button[i].r.right &&
            y >= g.button[i].r.bot &&
            y <= g.button[i].r.top) {
            g.button[i].over=1;
            if (g.button[i].over) {
                if (lbutton) {
                    switch (i) {
                        case 0:
                            resetGame();
                            break;
                        case 1:
                            cleanupSound();
                            quitGame();
                            break;
                        case 2:
                            g.show_credits = toggleCredits(g.show_credits);
                            break;

                    }
                }
            }
        }
    }
    return 0;
}

void getGridCenter(const int i, const int j, int cent[2])
{
    //This function can be optimized, and made more generic.
    int b2 = g.boardDim/2;
    int screenCenter[2] = {g.xres/2, g.yres/2};
    int s0 = screenCenter[0];
    int s1 = screenCenter[1];
    int bq;
    //quad upper-left corner
    int quad[2];
    //bq is the width of one grid section
    bq = (g.boardDim / g.gridDim) ;
    //-------------------------------------
    //because y dimension is bottom-to-top in OpenGL.
    int i1 = g.gridDim - i - 1;
    quad[0] = s0-b2;
    quad[1] = s1-b2;
    cent[0] = quad[0] + bq/2;
    cent[1] = quad[1] + bq/2;
    cent[0] += (bq * j);
    cent[1] += (bq * i1);
}

void physics(void)
{
    
int i;
    if (g.gameover)
        return;
    //Is it time to move the snake?
    static struct timespec snakeTime;
    static int firsttime=1;
    if (firsttime) {
        firsttime=0;
        clock_gettime(CLOCK_REALTIME, &snakeTime);

     }
    struct timespec tt;
    clock_gettime(CLOCK_REALTIME, &tt);
    double timeSpan = timeDiff(&snakeTime, &tt);
    if (timeSpan < g.pacman.delay)
        return;
    timeCopy(&snakeTime, &tt);
    //
    //playSound(g.alSourceDrip);
    //move the snake segments...
    int headpos[2];
    int newpos[2];
    int oldpos[2];
    //save the head position.
    headpos[0] = g.pacman.pos[0][0];
    headpos[1] = g.pacman.pos[0][1];
    printf("\n%d %d", g.pacman.pos[0][0], g.pacman.pos[0][1]);
    //pacman.direction:
    //0=down
    //1=left
    //2=up
    //3=right
    //Kenneths work, this is ugly.
    //
    switch (g.pacman.direction) {
        case DIRECTION_DOWN:  if(isValidVert(g.pacman.pos[0][0])){
                                    g.pacman.pos[0][1] +=1;
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
                                    g.pacman.pos[0][1] -=1;
                              }
                              break;


                          
                              
        case DIRECTION_LEFT:  if(isValidHor(g.pacman.pos[0][1])){
                                  g.pacman.pos[0][0] -=1;
                                  g.a=-5;
                                  g.s=-5;
                                  g.d=-5;
                                  g.f=43;
                                  g.g=43;
                                  g.h=43;
                                  g.q=-5;
                                  g.w=43;
                                  horWalls(g.pacman.pos[0][1]);
                                  if (g.pacman.pos[0][0] == -3 && g.pacman.pos[0][1] == 17)
                                     g.pacman.pos[0][0] = 39;
                                  if((g.pacman.pos[0][0] == g.a) || (g.pacman.pos[0][0] == g.s) || (g.pacman.pos[0][0] == g.d)
                                            || (g.pacman.pos[0][0] == g.f) ||(g.pacman.pos[0][0] == g.g) ||(g.pacman.pos[0][0] == g.h)
                                            ||  (g.pacman.pos[0][0] == g.q)|| (g.pacman.pos[0][0] == g.w))
                                            g.pacman.pos[0][0] +=1;
                              }
                                
                              break;
    

                              
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

        case DIRECTION_RIGHT: 
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

    newpos[0] = headpos[0];
    newpos[1] = headpos[1];
    for (i=1; i<g.pacman.length; i++) {
        oldpos[0] = g.pacman.pos[i][0];
        oldpos[1] = g.pacman.pos[i][1];
        if (g.pacman.pos[i][0] == newpos[0] &&
            g.pacman.pos[i][1] == newpos[1])
            break;
        g.pacman.pos[i][0] = newpos[0];
        g.pacman.pos[i][1] = newpos[1];
        newpos[0] = oldpos[0];
        newpos[1] = oldpos[1];
    }
    //did the snake eat the rat???

    
    
    for (i=0; i <2020; i++){
        if (headpos[0] == g.pellets[i].pos[0] && headpos[1] == g.pellets[i].pos[1]) {
            //yes, increase length of snake.
            //playSound(g.alSourceTick);
            if( g.pellets[i].status ==1){
            g.score++;
            g.pellets[i].status = 0;
            }

        //put new segment at end of snake.
       /*
        Log("snake ate rat. snake.length: %i   dir: %i\n",
                                        g.snake.length,g.snake.direction);
        int addlength = rand() % 4 + 4;
        for (i=0; i<addlength; i++) {
            g.snake.pos[g.snake.length][0] = g.snake.pos[g.snake.length-1][0];
            g.snake.pos[g.snake.length][1] = g.snake.pos[g.snake.length-1][1];
            g.snake.length++;
        }
       */
        //new position for rat...
        int collision=0;
        int ntries=0;
        while (1) {
            //for (int j = 0; j<10; j++){
            //g.rats[i].status = 0;
            //g.rats[j].pos[0] = -1;
            //g.rats[j].pos[1] = -1;

            collision=0;
            //}
            for (i=0; i<g.pacman.length; i++) {
                if (g.pellets[0].pos[0] == g.pacman.pos[i][0] &&
                    g.pellets[0].pos[1] == g.pacman.pos[i][1]) {
                    collision=1;
                    break;
                }
            }

            if (!collision) break;
            if (++ntries > 1000000) break;


        }
     
        return;


        }
    }
}


extern void show_dominics_credits(int x, int y);
extern void show_andrew_credits(int x, int y);
extern void show_kenneth_credits(int x, int y);
extern void show_juan_credits(int, int);
 extern void mainDisplay(void);
extern void credit_screen(void);

void render(void)
{
    int i;
    Rect r;
    //--------------------------------------------------------
    //This code is repeated several times in this program, so
    //it can be made more generic and cleaner with some work.
    int b2 = g.boardDim/2;
    int s0 = g.xres>>1;
    int s1 = g.yres>>1;

    
    //center of a grid
    int cent[2];
    //bq is the width of one grid section
    //--------------------------------------------------------
    //start the opengl stuff
    //set the viewing area on screen
    glViewport(0, 0, g.xres, g.yres);
    //clear color buffer
    glClearColor(0.1f, 0.2f, 0.3f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    //init matrices
    glMatrixMode (GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //this sets to 2D mode (no perspective)
    glOrtho(0, g.xres, 0, g.yres, -1, 1);
    //screen background
    glColor3f(0.5f, 0.5f, 0.5f);
    //glBindTexture(GL_TEXTURE_2D, g.marbleTexture);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(0, 0);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(0, g.yres);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(g.xres, g.yres);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(g.xres, 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    //draw all buttons
    for (i=0; i<g.nbuttons; i++) {
        if (g.button[i].over) {
            int w=2;
            glColor3f(1.0f, 1.0f, 0.0f);
            //draw a highlight around button
            glLineWidth(3);
            glBegin(GL_LINE_LOOP);
                glVertex2i(g.button[i].r.left-w,  g.button[i].r.bot-w);
                glVertex2i(g.button[i].r.left-w,  g.button[i].r.top+w);
                glVertex2i(g.button[i].r.right+w, g.button[i].r.top+w);
                glVertex2i(g.button[i].r.right+w, g.button[i].r.bot-w);
                glVertex2i(g.button[i].r.left-w,  g.button[i].r.bot-w);
            glEnd();
            glLineWidth(1);
        }
        if (g.button[i].down) {
            glColor3fv(g.button[i].dcolor);
        } else {
            glColor3fv(g.button[i].color);
        }
        glBegin(GL_QUADS);
            glVertex2i(g.button[i].r.left,  g.button[i].r.bot);
            glVertex2i(g.button[i].r.left,  g.button[i].r.top);
            glVertex2i(g.button[i].r.right, g.button[i].r.top);
            glVertex2i(g.button[i].r.right, g.button[i].r.bot);
        glEnd();
        r.left = g.button[i].r.centerx;
        r.bot  = g.button[i].r.centery-8;
        r.center = 1;
        if (g.button[i].down) {
            ggprint16(&r, 0, g.button[i].text_color, "Pressed!");
        } else {
            ggprint16(&r, 0, g.button[i].text_color, g.button[i].text);
        }
    }
    
    //draw the main game board in middle of screen
    glColor3f(0.6f, 0.5f, 0.2f);
    glBegin(GL_QUADS);
        glVertex2i(s0-b2-20, s1-(b2+50));
        glVertex2i(s0-b2-20, s1+(b2+50));
        glVertex2i(s0+b2, s1+(b2+50));
        glVertex2i(s0+b2, s1-(b2+50));
    glEnd();
    /*
    //
    //grid lines...
     int x0 = s0-b2;
     int x1 = s0+b2;
     int y0 = s1-b2;
     int y1 = s1+b2;
     glColor3f(0.1f, 0.1f, 0.1f);
     glBegin(GL_LINES);
     for (i=1; i<g.gridDim; i++) {
         y0 += 10;
        glVertex2i(x0,y0);
         glVertex2i(x1,y0);
     }
     x0 = s0-b2;
     y0 = s1-b2;
     y1 = s1+b2;
     for (j=1; j<g.gridDim; j++) {
         x0 += 10;
         glVertex2i(x0,y0);
         glVertex2i(x0,y1);
     }
     glEnd();
    */
     
     // draw Pacman
    float c[3]={1.0f,1.0,0.5};
    float rgb[3];
    rgb[0] = -0.9 / (float)g.pacman.length;
    rgb[2] = -0.45 / (float)g.pacman.length;
    glColor3fv(c);
    //
    glBegin(GL_QUADS);
    for (i=0; i<g.pacman.length; i++) {
        getGridCenter(g.pacman.pos[i][1],g.pacman.pos[i][0],cent);
        glVertex2i(cent[0]-4, cent[1]-3);
        glVertex2i(cent[0]-4, cent[1]+4);
        glVertex2i(cent[0]+3, cent[1]+4);
        glVertex2i(cent[0]+3, cent[1]-3);
        c[0] += rgb[0];
        c[2] += rgb[2];
        glColor3fv(c);
    }
    glEnd();
   

    glColor3f(0.1f, 0.8f, 0.1f);
    glBegin(GL_QUADS);
    for (i=0; i<g.pacman.length; i++) {
        getGridCenter(g.pacman.pos[i][1],g.pacman.pos[i][0],cent);
        glVertex2i(cent[0]-4, cent[1]-3);
        glVertex2i(cent[0]-4, cent[1]+4);
        glVertex2i(cent[0]+3, cent[1]+4);
        glVertex2i(cent[0]+3, cent[1]-3);
        glColor3f(0.0f, 0.6f, 0.0f);
    }
    glEnd();

    //draw pellets...
    

    for (i= 0; i<2020; i++){
    	if( g.pellets[i].status ==1){
    		getGridCenter(g.pellets[i].pos[1],g.pellets[i].pos[0],cent);
            glColor3f(1.0, 1.0f, 0.0f);
            glBegin(GL_QUADS);
             	glVertex2i(cent[0]-4, cent[1]-3);
                glVertex2i(cent[0]-4, cent[1]+4);
                glVertex2i(cent[0]+3, cent[1]+4);
                glVertex2i(cent[0]+3, cent[1]-3);
            glEnd();
            glPopMatrix();
        }
    }
    
    for (i= 0; i<5; i++){
    	if( g.bigPellets[i].status ==1){
    		getGridCenter(g.bigPellets[i].pos[1],g.bigPellets[i].pos[0],cent);
            glColor3f(1.0, 0.0f, 0.0f);
            glBegin(GL_QUADS);
             	glVertex2i(cent[0]-4, cent[1]-3);
                glVertex2i(cent[0]-4, cent[1]+4);
                glVertex2i(cent[0]+3, cent[1]+4);
                glVertex2i(cent[0]+3, cent[1]-3);
            glEnd();
            glPopMatrix();
        }
    }

    r.left   = g.xres/2;
    r.bot    = g.yres-100;
    r.center = 1;
    ggprint16(&r, 16, 0x00ffffff, "score: %d", g.score);

    //========================
    // Render the tile system
    //========================
    int tx = lev.tilesize[0];
    int ty = lev.tilesize[1];
    Flt dd = lev.ftsz[0];
    Flt offy = lev.tile_base-47;
    int ncols_to_render = 86;
    int col = (int)(g.camera[0] / dd);
    col = col % lev.ncols;
    //Partial tile offset must be determined here.
    //The leftmost tile might be partially off-screen.
    //cdd: camera position in terms of tiles.
    Flt cdd = g.camera[0] / dd-.05;
    //flo: just the integer portion
    Flt flo = floor(cdd);
    //dec: just the decimal portion
    Flt dec = (cdd - flo);
    //offx: the offset to the left of the screen to start drawing tiles
    Flt offx = -dec * dd - 10;
    //Log("gl.camera[0]: %lf   offx: %lf\n",gl.camera[0],offx);
    
    for (int j=0; j<ncols_to_render; j++) {
        int row = lev.nrows-1;
        for (int i=0; i<lev.nrows; i++) {
            if (lev.arr[row][col] == '#') {
                glColor3f(0.2, 0.2, 0.9);
                glPushMatrix();
                glTranslated((Flt)j*dd+offx + 200, (Flt)i*lev.ftsz[1]+offy + 100, 0);
                glBegin(GL_QUADS);
                    glVertex2i( 0,  0);
                    glVertex2i( 0, ty);
                    glVertex2i(tx, ty);
                    glVertex2i(tx,  0);
                glEnd();
                glPopMatrix();
                }
                --row;
            }
        
     col = (col+1) % lev.ncols;
    }
    
    if (g.show_credits) {
        //credit_screen();
        // r.bot = g.yres -20;
        // r.left = 10;
        // r.center = 0;
        // ggprint8b(&r, 16, 0x00A020F0, "Credits:");
        // show_dominics_credits(50, g.yres - 50);
        // show_andrew_credits(50, g.yres - 80);
        // show_kenneth_credits(50, g.yres - 110);
        // show_juan_credits(50, g.yres - 140);
    }
}

