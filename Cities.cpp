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
}

MobileCity::MobileCity(std::string cityName, int sz, std::string txturName)
: City(cityName, sz, txturName)
{
    sprite.setTextureRect(sf::IntRect(0, 0, 50, 100));
    sprite.setOrigin(25, 50);
}

void MobileCity::move(std::string direction)
{
    int dir = 0;
    if (direction.compare("forward") == 0)
        dir = -1;
    else if (direction.compare("backward") == 0)
        dir = 1;

    double radians = angle * PI / 180;

    y += (dir * std::cos(radians) * moveSpd);
    x += (dir * std::sin(radians) * moveSpd);
}

void MobileCity::rotate(std::string turn)
{
    if (turn.compare("right") == 0)
    {
        angle -= turn_rate;
    }
    else if (turn.compare("left") == 0)
    {
        angle += turn_rate;
    }
}

void MobileCity::update()
{
    sprite.setPosition(320, 320);
    sprite.setRotation(-angle);
    if (angle > 360 || angle < -360)
    {
        angle = (int)angle % 360;
    }
}

StaticCity::StaticCity(std::string cityName, int sz, std::string txturName)
: City(cityName, sz, txturName)
{
    x = rand() % 5000;
    y = rand() % 5000;
    std::cout << "Placed at (" << x << ", " << y << ")\n";
    sprite.setPosition(x, y);
    sprite.setTextureRect(sf::IntRect(0, 0, 100, 100));
    sprite.setOrigin(50, 50);
}

void StaticCity::update(PlayerCity playerCity)
{
    sprite.setPosition(x - playerCity.x, y - playerCity.y);
}

PlayerCity::PlayerCity(std::string cityName, int sz, std::string txturName)
: MobileCity(cityName, sz, txturName)
{
    sprite.setPosition(320, 320);
}

