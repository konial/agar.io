#ifndef _BALL_
#define _BALL_

#include "entity.h"
#include <list>

using namespace std;

class Ball : public Entity
{
private:
    Image image;
    Texture texture;
protected:
    Vector2f direction;
    float speed_;
    float accel_;
public:
    bool isShown_;
    Ball(double x, double y, int weight, double plX, double plY, float speed, float accel, int numb) : Entity(x, y, weight, plX, plY) {
        //image.loadFromFile("trump.jpg");
        //texture.loadFromImage(image);
        //body.setTexture(&texture);
        body.setFillColor(setColor(numb));
        speed_ = speed;
        accel_ = accel;
        //body.setTextureRect(sf::IntRect(200, 0, 700, 700));
        isShown_ = true;
    }
    double setDirection(Vector2f mouseCoords, bool flag);
    Vector2f getDirection() {return direction;}
    void update(double Time, bool now, Vector2f mouseCoords, double plX, double plY, list<Ball*> b);
    void changePos(double x, double y);
    void changeWeight(int mass);
    void setPosition(double x, double y) {coordinates.x = x; coordinates.y = y;}
    void changeBody();
    Color setColor(int c);
    float getSpeed() {return speed_;}
    friend bool Compare(Ball*& l, Ball*& r);
};

#endif // _BALL_
