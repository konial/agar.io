#include <SFML/Graphics.hpp>
#include <iostream>
#include "constants.h"
#include "ball.h"
#include "math.h"

bool meet(Ball* part1, Ball* part2)
{
    return(sqrt((part1->getPosition().x - part2->getPosition().x) * (part1->getPosition().x - part2->getPosition().x)
       + (part1->getPosition().y - part2->getPosition().y) * (part1->getPosition().y - part2->getPosition().y)) <
           part1->getRadius() + part2->getRadius());
}

sf::Color Ball::setColor(int c)
{
    switch(c) {
        case 0:
            return Color::Red;
            break;
        case 1:
            return Color::Magenta;
            break;
        case 2:
            return Color::Blue;
            break;
        case 3:
            return Color::Cyan;
            break;
        case 4:
            return Color::Green;
            break;
        case 5:
            return Color::Yellow;
            break;
    }
}

void qwe(Ball* minBall, Ball* maxBall, double plX, double plY)
{
    double x = minBall->getPosition().x - maxBall->getPosition().x;
    double y = minBall->getPosition().y - maxBall->getPosition().y;
    double dist = sqrt(x * x + y * y);
    double dx = minBall->getRadius() + maxBall->getRadius() - dist;
    if(y == 0) {
        minBall->setPosition(minBall->getPosition().x + Sign(x) * dx, minBall->getPosition().y);
        minBall->setBody(960 + minBall->getPosition().x - plX - minBall->getRadius(),
                            540 + minBall->getPosition().y - plY - minBall->getRadius());
        return;
    }
    double q = abs(x / y);
    double ddx, ddy;
    ddy = dx / (sqrt(q * q + 1));
    ddx = sqrt(dx * dx - ddy * ddy);
    minBall->setPosition(minBall->getPosition().x + Sign(x) * ddx,
                         minBall->getPosition().y + Sign(y) * ddy);
    minBall->setBody(960 + minBall->getPosition().x - plX - minBall->getRadius(),
                        540 + minBall->getPosition().y - plY - minBall->getRadius());
}

double Ball::setDirection(Vector2f mouseCoords, bool flag)
{
    double x = mouseCoords.x - getXWindowCoordinate() - radius_;
    double y = mouseCoords.y - getYWindowCoordinate() - radius_;
    double sp;
    double arg = weight_ / 3333 + ((double)(weight_ % 3333) / 3333) + 0.2;
    sp = 1 / arg + 2.5;
    if(accel_ == 0 && !flag) {speed_ = sp;}
    if(y == 0) {
        direction.x = Sign(x) * speed_;
        direction.y = 0;
        return sp;
    }
    double q = abs(x / y);
    direction.y = Sign(y) * (speed_ / (sqrt(q * q + 1)));
    direction.x = Sign(x) * (sqrt(speed_ * speed_ - direction.y * direction.y));
    return sp;
}

void Ball::update(double Time, bool now, Vector2f mouseCoords, double plX, double plY, list<Ball*> b)
{
    body.setRadius(radius_);
    bool flag = false;
    if(!now) {
        for(list<Ball*>::iterator it = b.begin(); it != b.end(); ++it) {
            if(this != (*it) && this->accel_ == 0 && (*it)->accel_ == 0) {
                if(meet(this, (*it))) {
                    qwe(this, (*it), plX, plY);
                }
            }
        }
    }
    double sp = setDirection(mouseCoords, false);
    if(!flag) {
        body.setPosition(960 + coordinates.x - plX - radius_, 540 + coordinates.y - plY - radius_);
        changePos(Time * direction.x, Time * direction.y);
    }
    if(speed_ > sp) {speed_ -= accel_;}
    else {accel_ = 0;}
}

void Ball::changePos(double x, double y)
{
    coordinates.x += x;
    coordinates.y += y;
}

void Ball::changeWeight(int mass)
{
    weight_ += mass;
    double dr = mass / 5 + ((double)(mass % 5) / 5);
    radius_ += dr;
    setBody(getXWindowCoordinate() - dr, getYWindowCoordinate() - dr);
}

void Ball::changeBody()
{
    weight_ /= 2;
    radius_ = weight_ / 5 + ((double)(weight_ % 5) / 5) + 10;
    body.setRadius(radius_);
}
