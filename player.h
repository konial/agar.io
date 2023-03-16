#ifndef _PLAYER_
#define _PLAYER_

#include <SFML/Graphics.hpp>
#include "constants.h"
#include <list>
#include "food.h"
#include "ball.h"

using namespace std;
using namespace sf;

class Player
{
private:
    list<Ball*> parts;
    Vector2f centerOfMass;
    Vector2f directionOfCenterMass;
    int numb;
public:
    int time_, max_time_;
    bool now;
    Player(int weight) {
        centerOfMass.x = rand() % MAP_WIDTH;
        centerOfMass.y = rand() % MAP_HEIGHT;
        numb = rand() % 6;
        Ball* ball = new Ball(centerOfMass.x, centerOfMass.y,
                              weight, centerOfMass.x, centerOfMass.y, 1.5, 0, numb);
        parts.push_back(ball);
        now = false;
        time_ = 0;
        max_time_ = 1e4;
    }
    friend void setDirection(Vector2i mouseCoords, Ball part);
    void addBall(list<Ball*> li);
    void mergeBall();
    list<Ball*> getParts() {return parts;}
    void setParts(list<Ball*> p) {parts = p;}
    void sortParts();
    int getWeight();
    void update();
    void clean();
    void crash(Ball* bl);
    void setCenter(double x, double y) {centerOfMass.x = x; centerOfMass.y = y;}
    void setDirection(double x, double y) {directionOfCenterMass.x = x; directionOfCenterMass.y = y;}
    Vector2f getCenter() {return centerOfMass;}
    Vector2f getDirection() {return directionOfCenterMass;}
};

#endif // _PLAYER_
