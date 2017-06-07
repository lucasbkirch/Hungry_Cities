#ifndef CITIES_H
#define CITIES_H
#include "Cities.h"
#endif // CITIES_H

//City BEGIN ---------------------------------------------------------------------

City::City(std::string cityName, int sz, std::string txturName)
{
        size_ = sz;
        name = cityName;
        texture.loadFromFile(txturName);
        sprite.setTexture(texture);
}

//City END ---------------------------------------------------------------------
//StaticCity BEGIN -------------------------------------------------------------

StaticCity::StaticCity(std::string cityName, int sz, std::string txturName)
: City(cityName, sz, txturName)
{
    x = rand() % screenSizeSpawnArea + 250;
    y = rand() % screenSizeSpawnArea + 250;
    std::cout << "Placed at (" << x << ", " << y << ")\n";
    sprite.setPosition(x, y);
    sprite.setTextureRect(sf::IntRect(0, 0, 100, 100));
    sprite.setOrigin(25, 50);
}

std::list<TerrainTile *> * StaticCity::execute(std::list<TerrainTile *> *)
{
    return NULL;
}

//StaticCity END ----------------------------------------------------------
//MobileCity BEGIN --------------------------------------------------------

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

    if (x > screenSize)
        x = screenSize;
    else if (x < 0)
        x = 0;

    if (y > screenSize)
        y = screenSize;
    else if (y < 0)
        y = 0;
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
//PlayerCity END --------------------------------------------------------
//AICity BEGIN ----------------------------------------------------------

void AICity::behaviorManagement()
{
    switch(currState){
        case 1:
            pursue();
            break;
        case 2:
            flee();
            break;
        case 3:
            idle();
            break;
        case 4:
            wander();
            break;
    }

    //TODO update current state!
}

void AICity::pursue()
{
    move("forward");
}

void AICity::flee()
{
    move("backward");
}

void AICity::idle()
{
    //Remain Idle
}

void AICity::wander()
{
    move("forward");
    rotate("left");
}

void AICity::updateCurrState(City * otherCity)
{
    if (currState == 3 || currState == 4) // If idle or wandering
        if (otherCity->size_ < size_)
        {
            currState = 1; //Pursue
        }
        else if (otherCity->size_ > size_)
        {
            currState = 2; // Flee
        }
}


std::list<TerrainTile *> * AICity::update(std::list<TerrainTile *> * collisions)
{
    fovSprite.setPosition(x, y);
    return MobileCity::update(collisions);
}

std::list<TerrainTile *> * AICity::execute(std::list<TerrainTile *> * collisions)
{
    behaviorManagement();
    return update(collisions);
}

//AICity END ------------------------------------------------------------
