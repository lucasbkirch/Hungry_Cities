#ifndef CITIES_H
#define CITIES_H
#include "Cities.h"
#endif // CITIES_H

//StaticCity BEGIN -------------------------------------------------------------

std::list<TerrainTile *> * StaticCity::execute(std::list<TerrainTile *> *)
{
    return NULL;
}

//StaticCity END ----------------------------------------------------------
//MobileCity BEGIN --------------------------------------------------------

void MobileCity::movement(int direction)
{
    double radians = angle * PI / 180;

    y += (direction * std::cos(radians) * moveSpd);
    x += (direction * std::sin(radians) * moveSpd);

    if (x > screenSize)
        x = screenSize;
    else if (x < 0)
        x = 0;

    if (y > screenSize)
        y = screenSize;
    else if (y < 0)
        y = 0;
}

void MobileCity::rotation(int turn)
{
    angle = angle + (turn * turn_rate);

    if (angle >= 180 || angle <= -180)
    {
        angle = -angle;
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

    return terrainSpeedCalculation(collisions);
}

std::list<TerrainTile *> * MobileCity::terrainSpeedCalculation(std::list<TerrainTile *> * collisions)
{
    if (collisions == NULL)
        return NULL;

    std::list<TerrainTile *> * grassTiles = new std::list<TerrainTile *>();
    std::list<TerrainTile *>::iterator collisionsIter;
    double modifierSum = 0;

    for (collisionsIter = collisions->begin(); collisionsIter != collisions->end(); collisionsIter++)
    {
        modifierSum += calculateSpeedMod((*collisionsIter)->speedModifier, (*collisionsIter)->type);
        if((*collisionsIter)->type.compare("grass") == 0 && (*collisionsIter)->tile.getGlobalBounds().intersects(wheelTracksSprite.getGlobalBounds()))
        {
            grassTiles->push_back((*collisionsIter));
        }
    }
    moveSpd = (modifierSum / collisions->size());
    return grassTiles;
}

//MobileCity END --------------------------------------------------------
//PlayerCity BEGIN ------------------------------------------------------

void PlayerCity::keyPressManagement()
{
    bool up_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    bool down_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    bool left_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    bool right_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);

    if (up_pressed)
    {
        movement(FORWARD);
    }
    else if (down_pressed)
    {
        movement(BACKWARD);
    }

    if (left_pressed && (up_pressed ^ down_pressed))
    {
        rotation(LEFT);
    }
    else if (right_pressed && (up_pressed ^ down_pressed))
    {
        rotation(RIGHT);
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
//PlayerCity END --------------------------------------------------------
