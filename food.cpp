#include <SFML/Graphics.hpp>
#include "food.h"

sf::Color Food::setColor(int c)
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
