#include "Cities.h"
#include <cmath>
#define PI 3.14159265

City::City(int sz, std::string txtur_name)
: mobileObj()
{
    texture.loadFromFile(txtur_name);
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, 50, 100));
    sprite.setPosition(320, 320);
    sprite.setOrigin(25, 50);
    sprite.setRotation(180); //TODO ???
}

void City::move(std::string direction)
{
    int dir = 0;
    if (direction.compare("forward") == 0)
        dir = -1;
    else if (direction.compare("backward") == 0)
        dir = 1;

    double radians = mobileObj.angle * PI / 180;

    mobileObj.y += (dir * std::cos(radians) * mobileObj.moveSpd);
    mobileObj.x += (dir * std::sin(radians) * mobileObj.moveSpd);
}

void City::rotate(std::string turn)
{
    if (turn.compare("right") == 0)
    {
        mobileObj.angle -= mobileObj.turn_rate;
    }
    else if (turn.compare("left") == 0)
    {
        mobileObj.angle += mobileObj.turn_rate;
    }
}

void City::update()
{
    sprite.setPosition(320, 320);
    sprite.setRotation(-mobileObj.angle);
    if (mobileObj.angle > 360 || mobileObj.angle < -360)
    {
        mobileObj.angle = (int)mobileObj.angle % 360;
    }
}
