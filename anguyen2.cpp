/* 
Creator: Andrew Nguyen
File name: anguyen2.cpp 
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#include "fonts.h"
void show_andrew_credits(int x, int y)
{
    Rect r;
    r.bot = y;
    r.left = x;
    r.center = 1;
    ggprint8b(&r, 16, 0x00ff0000, "Andrew");
}

int score=0,level=1;

void maxScore()
{
    if (level==1){
        score=180;
    }else if (level==2){
        score=180;
    }else if (level==3){
        score=180;
    }else if (level==4){
        score=270;
    }else if (level==5){
        score=270;
    }
}

class ghost{
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

ghost::ghost(int type) {
	y = 4;
	x = 5+type;
	direction = 4;
	if(type != 1) {
		active = false;
	} else {
		active = true;
	}
}

int ghost::getGhostX() {
	return x;
}
int ghost::getGhostY() {
	return y;
}
int ghost::getGhostDirection() {
	return direction;
}
void ghost::changeGhostDirection(int newDirection) {
	direction = newDirection;
}
void ghost::changePosition(int changeX, int changeY) {
    x = x + changeX;
    y = y + changeY;
}

/*
int main(){
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
}
*/
