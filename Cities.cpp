#ifndef CITIES_H
#define CITIES_H
#include "Cities.h"
#endif // CITIES_H

City::City(std::string cityName, int sz, std::string txturName)
{
        size_ = sz;
        name = cityName;
        texture.loadFromFile(txturName);
        sprite.setTexture(texture);
}

StaticCity::StaticCity(std::string cityName, int sz, std::string txturName)
: City(cityName, sz, txturName)
{
    x = rand() % 5000;
    y = rand() % 5000;
    std::cout << "Placed at (" << x << ", " << y << ")\n";
    sprite.setPosition(x, y);
    sprite.setTextureRect(sf::IntRect(0, 0, 100, 100));
    sprite.setOrigin(25, 50);
}

MobileCity::MobileCity(std::string cityName, int sz, std::string txturName)
: City(cityName, sz, txturName)
{
    sprite.setTextureRect(sf::IntRect(0, 0, 50, 100));
    wheelTracksSprite.setTextureRect(sf::IntRect(0, 80, 10, 20));
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

double MobileCity::calculateSpeedMod(double terrainSpeedMod, std::string terrainType)
{
    return terrainSpeedMod;
}

std::list<TerrainTile *> * MobileCity::update(std::list<TerrainTile *> * collisions)
{
    sprite.setPosition(x, y);
    sprite.setRotation(-angle);

    wheelTracksSprite.setPosition(x, y); //TODO affect so it is at the end of the city
    wheelTracksSprite.setRotation(-angle);

    if (angle > 360 || angle < -360)
    {
        angle = (int)angle % 360;
    }

    return terrainSpeedCalculation(collisions);
}

std::list<TerrainTile *> * MobileCity::terrainSpeedCalculation(std::list<TerrainTile *> * collisions)
{
        std::list<TerrainTile *> * grassTiles = new std::list<TerrainTile *>();
        std::list<TerrainTile *>::iterator collisionsIter;
        double modifierSum = 0;

        //sf::Image dirtImage;
        //dirtImage.loadFromFile("Images/dirt_terrain.png");

        for (collisionsIter = collisions->begin(); collisionsIter != collisions->end(); collisionsIter++)
        {
            modifierSum += calculateSpeedMod((*collisionsIter)->speedModifier, (*collisionsIter)->type);
            if((*collisionsIter)->type.compare("grass") == 0 && (*collisionsIter)->tile.getGlobalBounds().intersects(wheelTracksSprite.getGlobalBounds()))
            {

                grassTiles->push_back((*collisionsIter));

                //(*collisionsIter)->setTerrainType("dirt");
                //texture.update(dirtImage, (*collisionsIter)->x, (*collisionsIter)->y);
            }
        }
        moveSpd = (modifierSum / collisions->size());
        return grassTiles;
}

PlayerCity::PlayerCity(std::string cityName, int sz, std::string txtureName): MobileCity(cityName, sz, txtureName)
{
    x = 2500;
    y = 2500;
    sprite.setPosition(x, y);
}

void PlayerCity::keyPressManagement()
{
    bool up_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    bool down_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    bool left_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    bool right_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);

    if (up_pressed)
    {
        move("forward");
    }
    else if (down_pressed)
    {
        move("backward");
    }

    if (left_pressed && (up_pressed ^ down_pressed))
    {
        rotate("left");
    }
    else if (right_pressed && (up_pressed ^ down_pressed))
    {
        rotate("right");
    }
}

std::list<TerrainTile *> * PlayerCity::update(std::list<TerrainTile *> * collisions)
{
    return MobileCity::update(collisions);
}

std::list<TerrainTile *> * PlayerCity::execute(std::list<TerrainTile *> * collisions)
{
    keyPressManagement();
    return update(collisions);
}

std::list<TerrainTile *> * AICity::update(double x, double y, std::list<TerrainTile *> * collisions)
{
    fovSprite.setPosition(x, y);
    return MobileCity::update(collisions);
}

