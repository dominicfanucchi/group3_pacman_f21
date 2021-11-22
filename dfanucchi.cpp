//Dominic Fanucchi
//program: dfanucchi.cpp
//author: Dominic Fanucchi
//date: September 2021

#include "fonts.h"
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <iostream>
//#include "dfanucchi.h"
#define USE_OPENAL_SOUND
#ifdef USE_OPENAL_SOUND
#include </usr/include/AL/alut.h>
#endif //USE_OPENAL_SOUND

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

struct credits_screen 
{
    int x, y;

    credits_screen() {
        x = 815;
        y = 800;
    }
}c;


void credit_screen(int x, int y){
    glViewport(0, 0, c.x, c.y);
    //clear color buffer
    glClearColor(0.5f, 0.2f, 0.3f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    //init matrices
    glMatrixMode (GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //this sets to 2D mode (no perspective)
    glOrtho(0, c.x, 0, c.y, -1, 1);
    //screen background
    glColor3f(0.5f, 0.5f, 0.5f);
    //glBindTexture(GL_TEXTURE_2D, g.marbleTexture);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(0, 0);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(0, c.y);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(c.x, c.y);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(c.x, 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
}

struct Sound 
{
		ALuint alBufferBeginning, alBufferChomp, alBufferDeath, alBufferEatFruit, alBufferEatGhost, alBufferExtraLife, alBufferIntermission; //pacman sound files
    ALuint alSourceBeginning, alSourceChomp, alSourceDeath, alSourceEatFruit, alSourceEatGhost, alSourceExtraLife, alSourceIntermission; //pacman sound files
} s;

void initSound()
{
    #ifdef USE_OPENAL_SOUND
    alutInit(0, NULL);
    if (alGetError() != AL_NO_ERROR) {
        printf("ERROR: alutInit()\n");
        return;
    }
    //Clear error state.
    alGetError();
    
    //Setup the listener.
    //Forward and up vectors are used.
    float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    alListenerfv(AL_ORIENTATION, vec);
    alListenerf(AL_GAIN, 1.0f);
    
    //Buffer holds the sound information.
    //pacman sound files
    g.alBufferBeginning = alutCreateBufferFromFile("./sounds/pacman_beginning.wav");
    g.alBufferChomp = alutCreateBufferFromFile("./sounds/pacman_chomp.wav");
    g.alBufferDeath = alutCreateBufferFromFile("./sounds/pacman_death.wav");
    g.alBufferEatFruit = alutCreateBufferFromFile("./sounds/pacman_eatfruit.wav");
    g.alBufferEatGhost = alutCreateBufferFromFile("./sounds/pacman_eatghost.wav");
    g.alBufferIntermission = alutCreateBufferFromFile("./sounds/pacman_intermission.wav");
    g.alBufferExtraLife = alutCreateBufferFromFile("./sounds/pacman_extrapac.wav");
    
    //Source refers to the sound.
    //Generate a source, and store it in a buffer.
    alGenSources(1, &g.alSourceBeginning);
    alSourcei(g.alSourceBeginning, AL_BUFFER, g.alBufferBeginning);
    //Set volume and pitch to normal, looping of sound.
    alSourcef(g.alSourceBeginning, AL_GAIN, 1.0f);
    alSourcef(g.alSourceBeginning, AL_PITCH, 1.0f);
    alSourcei(g.alSourceBeginning, AL_LOOPING, AL_TRUE);
    if (alGetError() != AL_NO_ERROR) {
        printf("ERROR: setting source\n");
        return;
    }
    #endif //USE_OPENAL_SOUND
}

void cleanupSound()
{
    #ifdef USE_OPENAL_SOUND
    //First delete the source.
    alDeleteSources(1, &g.alSourceBeginning);
    //Delete the buffer.
    alDeleteBuffers(1, &g.alSourceBeginning);
    //Close out OpenAL itself.
    //Get active context.
    ALCcontext *Context = alcGetCurrentContext();
    //Get device for active context.
    ALCdevice *Device = alcGetContextsDevice(Context);
    //Disable context.
    alcMakeContextCurrent(NULL);
    //Release context(s).
    alcDestroyContext(Context);
    //Close device.
    alcCloseDevice(Device);
    #endif //USE_OPENAL_SOUND
}

void playSound(ALuint source)
{
    #ifdef USE_OPENAL_SOUND
    alSourcePlay(source);
    #endif //USE_OPENAL_SOUND
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