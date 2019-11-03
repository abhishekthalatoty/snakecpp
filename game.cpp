#include <iostream>
#include <deque>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <graphics.h>

using namespace std;


//used for debugging purpose
template <class T>
void print(T s) {
    cout << s << endl;
}


int gamespeed = 250;    // number of milliseconds at which the game refreshes.


// use the following for directions
int right = 0;
int down = 1;
int left = 2;
int up = 3;


// this pixelizes the screen. its how the whole screen is divided into tiles or cells.
int cellwidth = 20;

int scrsizex = 640;
int scrsizey = 480;
int wallx1 = cellwidth;
int wallx2 = scrsizex - cellwidth;
int wally1 = cellwidth;
int wally2 = scrsizey - cellwidth;

void generateRandomFood();

class Point {
public:
    int x;
    int y;
};

class Snake {
public:
    deque<Point> body;
    int direction;


    Snake() {
        Point p;
        p.x = ::scrsizex / 2;
        p.y = ::scrsizey / 2;
        this->body.push_back(p);
        this->direction = ::left;
        for (int i = 1; i < 5; i++) {
            Point temp;
            temp.x = p.x + i * ::cellwidth;
            temp.y = p.y;
            this->body.push_back(temp);
        }
        generateRandomFood();
    }


    int getLength() {
        return this->body.size();
    }

    void moveSnake() {
        this->grow();
        this->body.pop_back();
    }

    void grow() {
        this->body.push_front(newHead());
    }

    void paintSnake() {
        int i;
        setfillstyle(1,15);
        for(i = 0; i< getLength(); i++) {
            rectangle(this->body[i].x, this->body[i].y, this->body[i].x + cellwidth, this->body[i].y + cellwidth);
            floodfill(this->body[i].x+1,this->body[i].y+1, 15 );
        }


    }

    Point newHead() {

        // this sets the snake direction based on user input
        if(GetAsyncKeyState(VK_RIGHT)) {
            direction=0;
        } else if(GetAsyncKeyState(VK_LEFT)) {
            direction=2;
        } else if(GetAsyncKeyState(VK_UP)) {
            direction=3;
        } else if(GetAsyncKeyState(VK_DOWN)) {
            direction=1;
        }

        Point temp;
        if (direction == ::right) {
            temp.x = this->body[0].x + ::cellwidth;
            temp.y = this->body[0].y;
        } else if (direction == ::down) {
            temp.x = this->body[0].x;
            temp.y = this->body[0].y + ::cellwidth;
        } else if (direction == ::left) {
            temp.x = this->body[0].x - ::cellwidth;
            temp.y = this->body[0].y;
        } else if (direction == ::up) {
            temp.x = this->body[0].x;
            temp.y = this->body[0].y - ::cellwidth;
        }
        return (temp);
    }
};



// Global snake and food declarations.
Point food;
Snake snake;



bool meet(Point p, Point q) {
    return (p.x == q.x && p.y == q.y);
}

bool collide() {
    return (::snake.body[0].y < wally1 + cellwidth || ::snake.body[0].y > wally2 - cellwidth ||
            ::snake.body[0].x < wallx1 + cellwidth || ::snake.body[0].x > wallx2 - cellwidth);
}

bool selfEat() {
    for (int i = 2; i < ::snake.getLength(); i++) {
        if (meet(::snake.body[0], ::snake.body[i])) {
            return true;
        }
    }
    return false;
}

bool checkFood() {
    return (meet(food, ::snake.body[0]));
}

void generateRandomFood() {
    food.x =(3 + (rand() % (wallx2 / cellwidth -5))) * cellwidth ;
    food.y = (3+ (rand() % (wally2 / cellwidth -5)))* cellwidth ;
}

void updateSnake() {
    if (selfEat()) {}
    else if (collide()) {}
    else if (checkFood()) {
        snake.grow();
        generateRandomFood();
    } else snake.moveSnake();
}

void drawFood() {
    setfillstyle(1,RED);
    rectangle(food.x, food.y, food.x + cellwidth, food.y + cellwidth);
    floodfill(food.x+1, food.y+1, 15);
}



int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");
    while (1) {
        rectangle(wallx1, wally1, wallx2, wally2);
        updateSnake();
        drawFood();
        snake.paintSnake();
        Sleep(gamespeed);
        cleardevice();
    }
    closegraph();
    getch();
    return 0;
}
