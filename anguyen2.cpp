
/************************************************************************
*   CMPS-3350                                                           *
*   Creator: Andrew Nguyen                                              *
*   File name: anguyen2.cpp                                             *
*   Objective: Desgin Scoring, Dashboard, pellets, ghost, pacman        *
*   Project prestation Dec 2                                            *
************************************************************************/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>

using namespace std;

#include "fonts.h"
void show_andrew_credits(int x, int y)
{
    Rect r;
    r.bot = y;
    r.left = x;
    r.center = 1;
    ggprint8b(&r, 16, 0x00ff0000, "Andrew Nguyen");
}

#ifndef pellet
#define pellet
class Pellet
{
public:
    //constructor sets pellets position
    Pellet(int pellet_type, int x, int y);
    
    //render pellet
    void render();
    
    //destroy pellet (when pacman eats it)
    void destroy();
};
#endif

// ERROR: expected expression 
//Constructor 

Pellet::Pellet(int pellet_type, int x, int y)
{
    //regular pellet
    //if (pellet_type == 1)
        //coin =  {(x + TILE_WIDTH/2 - 2), (y + TILE_HEIGHT/2 - 2), 4, 4};
    //big pellet
    //else if (pellet_type == 2)
        //coin = {(x + TILE_WIDTH/2 - 6), (y + TILE_HEIGHT/2 - 6), 14, 14};
    //if no pellet
    //else if (pellet_type == 0)
        //coin = {0, 0, 0, 0};
}
/*
//render pellet
void Pellet::render()
{
    Render( &coin);
}

//remove pellet
void Pellet::destroy()
{
    coin = {0, 0, 0, 0 };
}
*/

#ifndef collider
#define collider
class Collider
{
public:
    //constructor
    Collider();
    
    //coordinate accessors
    int get_x();
    int get_y();

    //dimension accessors
    int get_width();
    int get_height();

    bool collided(Collider object2);

    protected:
    //used to check for collisions
    int Coin;
    
};
#endif 

/*
Collider::Collider()
{
    Coin = { 0, 0, 0, 0 };
}

int Collider::get_x()
{
    return Coin.x;
}

int Collider::get_y()
{
    return Coin.y;
}

int Collider::get_width()
{
    return Coin.w;
}

int Collider::get_height()
{
    return Coin.h;
}
*/

//linker command failed 

//check to see if objects collided
/*bool Collider::collided(Collider object2)
{
    //if calling object is to the right of object 2. Cannot collided 
    if ( get_x() >= object2.get_x() + object2.get_width() )
        return false;
    
    //if calling object is to the left of object 2. Cannot collided 
    if ( get_x() <= object2.get_x() - get_width() )
        return false;
    
    //if calling object is below object 2. Cannot collided 
    if ( get_y() >= object2.get_y() + object2.get_height() )
        return false;
    
    //if calling object is above object 2. Cannot collided 
    if ( get_y() <= object2.get_y() - get_height() )
        return false;
    
    //otherwise, it must have collided with pacman or ghost 
    return true;
}*/

const int TILE_WIDTH  = 30;
const int TILE_HEIGHT = 30;
int coin; 

//Dashboard 
class Dashboard
{
public:
    void addScore(int ipt);
    void reset();

    int getScore();
    int getHighScore();

private:
    int high;
};

int Score=0,level=1;
//Scoring 
void maxScore()
{
    if (level==1){
        Score=100;
    }else if (level==2){
        Score=200;
    }else if (level==3){
        Score=200;
    }else if (level==4){
        Score=300;
    }else if (level==5){
        Score=300;
    }
}
// creating ghost
class ghost
{
	public:
		ghost(int type);
		int getGhostX();
		int getGhostY();
		int getGhostDirection();
		void changeGhostDirection(int newDirection);
        void changePosition(int changeX, int changeY);

	private:
		int x, y, direction;
		bool active, inactive;
};

ghost::ghost(int type)
{
	y = 4;
	x = 5+type;
	direction = 4;
	if(type != 1) {
		active = false;
	} else {
		active = true;
	}
}
// movements of the ghost 
int ghost::getGhostX() 
{
	return x;
}
int ghost::getGhostY() 
{
	return y;
}
int ghost::getGhostDirection() 
{
	return direction;
}
void ghost::changeGhostDirection(int newDirection) 
{
	direction = newDirection;
}
void ghost::changePosition(int changeX, int changeY) 
{
    x = x + changeX;
    y = y + changeY;
}

/*int main()
{
    char choice; 

    cout<<"MENU: "<<endl;
    cout<<"1 : CREATE ACCOUNT "<<endl;
    cout<<"2 : LOGIN"<<endl;
    cout<<"3 : DELETE ACCOUNT"<<endl;
    cout<<"4 : VIEW ACCOUNT DETAILS"<<endl;
    cout<<"5 : SCORES"<<endl;
    cout<<"6 : INSTRUCTIONS"<<endl;
    cout<<"7 : EXIT"<<endl<<endl;
    
    cout<<"Enter your choice: ";

    cin>>choice;
    
    return 0; 
}*/
