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
//AICity BEGIN ----------------------------------------------------------

void AICity::behaviorManagement()
{
    switch(currState){
        case PURSUE:
            pursue();
            break;
        case FLEE:
            flee();
            break;
        case IDLE:
            idle();
            break;
        case WANDER:
            wander();
            break;
    }
}

void AICity::pursue()
{
    goToDestPoint();

    if (abs(currDestPoint.first - x) < 50 && abs(currDestPoint.second - y) < 50) // If point is reached, wander
    {
        currState = WANDER;
        currTargetName = "";
        currDestPoint = std::make_pair<double, double>(-1, -1);
    }
}

void AICity::flee()
{

}

void AICity::idle()
{
    //Remain Idle
}

void AICity::wander()
{
    movement(FORWARD);
    rotation(LEFT);
}

void AICity::updateCurrState(City * otherCity)
{
    if (otherCity->size_ > size_) //FLEE
    {
        //Break off pursuit
        if (currState == PURSUE)
        {
            currTargetName = "";
            currDestPoint.first = -1;
            currDestPoint.second = -1;
        }

        currState = FLEE; // Flee

        //If already fleeing from that city, update the dangerPoint associated with it.
        std::map<std::string, std::pair<double, double>>::iterator dangerIter;
        for (dangerIter = currDangerPoints.begin(); dangerIter != currDangerPoints.end(); dangerIter++)
        {
            if (dangerIter->first.compare(otherCity->name))
            {
                currDangerPoints.erase(dangerIter->first);
                break;
            }
        }
        currDangerPoints.insert(std::make_pair(otherCity->name, std::make_pair(otherCity->sprite.getPosition().x, otherCity->sprite.getPosition().y)));
    }
    else if (otherCity->size_ < size_ && currTargetName.empty()) // PURSUE IF NOT CURRENTLY PURSUING
    {
        currState = PURSUE; //Pursue
        currTargetName = otherCity->name;
        currDestPoint.first = otherCity->sprite.getPosition().x;
        currDestPoint.second = otherCity->sprite.getPosition().y;
    }
    else if (currState == PURSUE && otherCity->name.compare(currTargetName) == 0) //UPDATE currTargetPoint position of targetCity
    {
        currDestPoint.first = otherCity->sprite.getPosition().x;
        currDestPoint.second = otherCity->sprite.getPosition().y;
    }
}

void AICity::goToDestPoint()
{
    if (currDestPoint.first < 0 || currDestPoint.second < 0)
    {
        currState = WANDER;
        return;
    }

    double targetAngle = calcRelativeAngle(std::make_pair(x, y), std::make_pair(currDestPoint.first, currDestPoint.second));

    if (fabs(angle - targetAngle) >= 2)
    {
        //if both positive or both negative
        if (angle * targetAngle >= 0)
        {
            if (angle > targetAngle)
                rotation(RIGHT);
            else
                rotation(LEFT);
        }
        else
        {
            double absDiff = 360 - fabs(angle) - fabs(targetAngle);

            if ((angle > targetAngle && absDiff >= 180) || (angle <= targetAngle && absDiff < 180))
                rotation(RIGHT);

            else
                rotation(LEFT);
        }
    }
    else
        movement(FORWARD);
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
