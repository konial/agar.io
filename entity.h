#ifndef _ENTITY_
#define _ENTITY_

#include <SFML/Graphics.hpp>

using namespace sf;

class Entity
{
protected:
    Vector2f coordinates;
    CircleShape body;
    int weight_;
    float radius_;
public:
    Entity(double x, double y, int weight, double plX, double plY) {
        coordinates.x = x;
        coordinates.y = y;
        weight_ = weight;
        radius_ = weight_ / 5 + ((double)(weight_ % 5) / 5) + 10;
        body.setRadius(radius_);
        body.setOutlineThickness(1);
        body.setOutlineColor(Color::White);
        body.setPosition(960 + x - plX - radius_, 540 + y - plY - radius_);
    }
    double getXWindowCoordinate() {return body.getPosition().x;}
    double getYWindowCoordinate() {return body.getPosition().y;}
    void setBody(double x, double y) {body.setPosition(x, y);}
    Vector2f getPosition() {return coordinates;}
    CircleShape getBody() {return body;}
    float getRadius() {return radius_;}
    float getWeight() {return weight_;}
};

#endif // _ENTITY_
