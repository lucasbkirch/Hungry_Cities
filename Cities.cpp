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
    goToDestPoint();

    if (abs(currDestPoint.first - x) < 50 && abs(currDestPoint.second - y) < 50) // If point is reached, wander
    {
        currState = 3;
        currTargetName = "";
        currDestPoint = std::make_pair<double, double>(-1, -1);
    }
}

void AICity::flee()
{
    //move("backward");
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
    if (otherCity->size_ > size_) //FLEE
    {
        //Break off pursuit
        if (currState == 1)
        {
            currTargetName = "";
            currDestPoint.first = -1;
            currDestPoint.second = -1;
        }

        currState = 2; // Flee

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
        currState = 1; //Pursue
        currTargetName = otherCity->name;
        currDestPoint.first = otherCity->sprite.getPosition().x;
        currDestPoint.second = otherCity->sprite.getPosition().y;
    }
    else if (currState == 1 && otherCity->name.compare(currTargetName) == 0) //UPDATE currTargetPoint position of targetCity
    {
        currDestPoint.first = otherCity->sprite.getPosition().x;
        currDestPoint.second = otherCity->sprite.getPosition().y;
    }
}

void AICity::goToDestPoint()
{
    if (currDestPoint.first < 0 || currDestPoint.second < 0)
    {
        currState = 3;
        return;
    }

    double targetAngle = relAngle(x, y, currDestPoint.first, currDestPoint.second);

    if (fabs(angle - targetAngle) >= 2)
    {
        //if both positive or both negative
        if (angle * targetAngle >= 0)
        {
            if (angle > targetAngle)
                rotate("right");
            else
                rotate("left");
        }
        else
        {
            double absDiff = 360 - fabs(angle) - fabs(targetAngle);

            if ((angle > targetAngle && absDiff >= 180) || (angle <= targetAngle && absDiff < 180))
                rotate("right");

            else
                rotate("left");
        //}
    }
    else
        move("forward");
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
