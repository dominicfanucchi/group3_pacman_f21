//program: dfanucchi.cpp
//author: Dominic Fanucchi
//date: September 2021

//#include "dfanucchi.h"
#include "fonts.h"
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <iostream>
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
} c;

void credit_screen()
{
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
    float vec[6] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    alListenerfv(AL_ORIENTATION, vec);
    alListenerf(AL_GAIN, 1.0f);
    
    //Buffer holds the sound information.
    //pacman sound files
    s.alBufferBeginning = alutCreateBufferFromFile("./sounds/pacman_beginning.wav");
    s.alBufferChomp = alutCreateBufferFromFile("./sounds/pacman_chomp.wav");
    s.alBufferDeath = alutCreateBufferFromFile("./sounds/pacman_death.wav");
    s.alBufferEatFruit = alutCreateBufferFromFile("./sounds/pacman_eatfruit.wav");
    s.alBufferEatGhost = alutCreateBufferFromFile("./sounds/pacman_eatghost.wav");
    s.alBufferIntermission = alutCreateBufferFromFile("./sounds/pacman_intermission.wav");
    s.alBufferExtraLife = alutCreateBufferFromFile("./sounds/pacman_extrapac.wav");
    
    //Source refers to the sound.
    //Generate a source, and store it in a buffer.
    alGenSources(1, &s.alSourceBeginning);
    alSourcei(s.alSourceBeginning, AL_BUFFER, s.alBufferBeginning);
    //Set volume and pitch to normal, looping of sound.
    alSourcef(s.alSourceBeginning, AL_GAIN, 1.0f);
    alSourcef(s.alSourceBeginning, AL_PITCH, 1.0f);
    alSourcei(s.alSourceBeginning, AL_LOOPING, AL_TRUE);
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
    alDeleteSources(1, &s.alSourceBeginning);
    //Delete the buffer.
    alDeleteBuffers(1, &s.alSourceBeginning);
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

int getHighScore()
{
    int highScore;
    fstream scoreFile("high_score.txt"); // Open a stream to the high score file
    if(scoreFile.good()) { // If the file exists get the value from inside
        scoreFile >> highScore;
        scoreFile.close();
        return highScore; // Return the score
    } else { 
        std::ofstream highScoreFile("high_score.txt"); // Create the file
        highScoreFile << 0; // Pass in the highscore of 0
        scoreFile.close();
        highScoreFile.close();
    }
    return 0; // Return 0 as the file hadnt previously existed
}

void setHighScore(int score)
{
    int currentHigh = getHighScore(); // Store the current high score
    if(currentHigh < score) { // Only update the high score if the current score is higher than the previous
        ofstream highFile("high_score.txt");
        highFile.clear();
        highFile << score;
        highFile.close();
    }
}
