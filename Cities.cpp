#include "Cities.h"
#include <cmath>
#include <iostream>
#define PI 3.14159265



City::City(std::string cityName, int sz, std::string txturName)
{
    size_ = sz;
    name = cityName;
    texture.loadFromFile(txturName);
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, 50, 100));
    sprite.setOrigin(25, 50);
    sprite.setRotation(180); //TODO ???
}

MobileCity::MobileCity(std::string cityName, int sz, std::string txturName)
: City(cityName, sz, txturName), mobileObj()
{
    //TODO
}

void MobileCity::move(std::string direction)
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

void MobileCity::rotate(std::string turn)
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

void MobileCity::update()
{
    sprite.setPosition(320, 320);
    sprite.setRotation(-mobileObj.angle);
    if (mobileObj.angle > 360 || mobileObj.angle < -360)
    {
        mobileObj.angle = (int)mobileObj.angle % 360;
    }
}

StaticCity::StaticCity(std::string cityName, int sz, std::string txturName)
: City(cityName, sz, txturName)
{
    sprite.setPosition(2500, 2500);
}

void StaticCity::update(PlayerCity playerCity)
{
    sprite.setPosition(2500 - playerCity.mobileObj.x, 2500 - playerCity.mobileObj.y);
}

PlayerCity::PlayerCity(std::string cityName, int sz, std::string txturName)
: MobileCity(cityName, sz, txturName)
{
    sprite.setPosition(320, 320);
}

