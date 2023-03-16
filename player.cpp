#include <SFML/Graphics.hpp>
#include <iostream>
#include "player.h"
#include "math.h"

using namespace std;
using namespace sf;

bool Compare(Ball*& l, Ball*& r)
{
    return(l->weight_ < r->weight_);
}

void Player::sortParts() {parts.sort(Compare);}

void Player::addBall(list<Ball*> li)
{
    list<Ball*> p;
    for(list<Ball*>::iterator it = li.begin(); it != li.end(); ++it) {
            (*it)->changeBody();
            Ball* ball = new Ball((*it)->getPosition().x, (*it)->getPosition().y,
                                  (*it)->getWeight(), centerOfMass.x, centerOfMass.y, 16, 0.05, numb);
            p.push_back(ball);
    }
    for(list<Ball*>::iterator it = p.begin(); it != p.end(); ++it)
        parts.push_back(*it);
    time_ = 0;
    max_time_ = 1e4 / parts.size();
}

void Player::mergeBall()
{
    list<Ball*> p;
    for(list<Ball*>::iterator it = parts.begin(); it != parts.end(); ++it) {
        for(list<Ball*>::iterator ir = parts.begin(); ir != parts.end(); ++ir) {
            if((*it)->isShown_ && (*ir)->isShown_) {
                if((sqrt(((*it)->getPosition().x - (*ir)->getPosition().x) * ((*it)->getPosition().x - (*ir)->getPosition().x)
                       + ((*it)->getPosition().y - (*ir)->getPosition().y) * ((*it)->getPosition().y - (*ir)->getPosition().y)) <
                       max((*it)->getRadius(), (*ir)->getRadius())) && (*it != *ir)) {
                    double x = ((*it)->getPosition().x * (*it)->getWeight() + (*ir)->getPosition().x * (*ir)->getWeight()) / ((*it)->getWeight() + (*ir)->getWeight());
                    double y = ((*it)->getPosition().y * (*it)->getWeight() + (*ir)->getPosition().y * (*ir)->getWeight()) / ((*it)->getWeight() + (*ir)->getWeight());
                    Ball* ball = new Ball(x, y, (*it)->getWeight() + (*ir)->getWeight(), centerOfMass.x, centerOfMass.y, 10, 0, numb);
                    (*it)->isShown_ = false;
                    (*ir)->isShown_ = false;
                    parts.push_back(ball);
                }
            }
        }
    }
}

int Player::getWeight()
{
    int weight = 0;
    for(list<Ball*>::iterator it = parts.begin(); it != parts.end(); ++it) {
        weight += (*it)->getWeight();
    }
    return weight;
}

void Player::update()
{
    if(parts.size() > 1 && !now) {
        time_++;
    }
    else if (parts.size() == 1 && now) {
        now = false;
        time_ == 0;
        max_time_ = 1e4;
    }
    if(time_ == max_time_) {
        now = true;
        mergeBall();
    }
    clean();
}

void Player::clean()
{
    list<Ball*> p;
    for(list<Ball*>::iterator ii = parts.begin(); ii != parts.end(); ++ii) {
        if((*ii)->isShown_)
            p.push_back(*ii);
    }
    setParts(p);
    sortParts();
}

void Player::crash(Ball* bl)
{
    list<Ball*> li;
    li.push_back(bl);
    while(li.size() + parts.size() - 1 < 8) {
        list<Ball*> p;
        for(list<Ball*>::iterator it = li.begin(); it != li.end(); ++it) {
                (*it)->changeBody();
                Ball* ball = new Ball((*it)->getPosition().x, (*it)->getPosition().y,
                                      (*it)->getWeight(), centerOfMass.x, centerOfMass.y, (*it)->getWeight() / 10, 0.05, numb);
                p.push_back(ball);
        }
        for(list<Ball*>::iterator it = p.begin(); it != p.end(); ++it)
            li.push_back(*it);
    }
    for(list<Ball*>::iterator it = li.begin(); it != li.end(); ++it) {
        if(*it != bl) {
            parts.push_back(*it);
        }
    }
    time_ = 0;
    max_time_ = 1e4 / parts.size();
}
