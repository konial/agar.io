#ifndef _FOOD_
#define _FOOD_

#include "entity.h"
#include "player.h"

class Food : public Entity
{
private:
    Color color_;
public:
    Food(double x, double y, int weight, double plX, double plY) : Entity(x, y, weight, plX, plY) {
        color_ = setColor(rand() % 6);
        body.setFillColor(color_);
    }
    Color setColor(int x);
};

#endif // _FOOD_
