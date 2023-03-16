#ifndef _BUSH_
#define _BUSH_

#include "entity.h"

class Bush : public Entity
{
private:
    Image image;
    Texture texture;
public:
    bool isShown_;
    Bush(double x, double y, int weight, double plX, double plY) : Entity(x, y, weight, plX, plY) {
        image.loadFromFile("bush.png");
        texture.loadFromImage(image);
        body.setTexture(&texture);
        body.setTextureRect(sf::IntRect(0, 0, 118, 118));
        isShown_ = true;
    }
    /*double setDirection(Vector2i mouseCoords, bool flag);
    Vector2f getDirection() {return direction;}
    void update(double Time, bool now, Vector2i mouseCoords, double plX, double plY, list<Ball*> b);
    void changePos(double x, double y);
    void changeWeight(int mass);
    void setPosition(double x, double y) {coordinates.x = x; coordinates.y = y;}
    void changeBody();
    float getSpeed() {return speed_;}
    friend bool Compare(Ball*& l, Ball*& r);*/
};

#endif // _BUSH_
