#ifndef AI_H
#define AI_H
#include "AI.h"
#endif // AI_H


void AIObject::update(double x, double y)
{
    fovSprite.setPosition(x, y);
}

double AIObject::calcRelativeAngle(std::pair<double, double> aiPos, std::pair<double, double> targetPos)
{
    return atan2(aiPos.first - targetPos.first, aiPos.second - targetPos.second) * 180 / PI;
}

double AIObject::inverseAngle(double pAngle)
{
    if (pAngle - 180 < -180)
        return pAngle + 180;
    return pAngle - 180;
}

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
        currState = IDLE;
        currTargetName = "";
        setCurrDestPoint(0, 0);
        movingForward = false;
    }
}

void AICity::flee()
{
    if ((abs(currDestPoint.first - x) < 50 && abs(currDestPoint.second - y) < 50) || currDangerPoints.size() < 1) // If point is reached, wander
    {
        currState = IDLE;
        currTargetName = "";
        setCurrDestPoint(0, 0);
        movingForward = false;
    }

    calcFleePoint();
    goToDestPoint();
}

void AICity::calcFleePoint()
{
        std::map<std::string, std::pair<double, double>>::iterator dangerIter;

        std::map<std::string, std::pair<double, double>>::iterator dangerIterOuter;

        double DangerAngleDiffArr[currDangerPoints.size()];
        int i = 0;

        for (dangerIter = currDangerPoints.begin(); dangerIter != currDangerPoints.end(); dangerIter++)
        {
            DangerAngleDiffArr[i] = calcRelativeAngle(std::make_pair(x, y), dangerIter->second);
            i++;
        }

        if (currDangerPoints.size() > 1)
        {
            double fleeDeg = 0;
            double fleeBound1 = 0;
            double fleeBound2 = 0;

            for (unsigned int j = 0; j < currDangerPoints.size(); j++)
                for (unsigned int h = 0; h < currDangerPoints.size(); h++)
                    if ((180 - fabs(fabs(DangerAngleDiffArr[h] - DangerAngleDiffArr[j]) - 180)) > fleeDeg)
                    {
                        std::cout << (180 - fabs(fabs(DangerAngleDiffArr[h] - DangerAngleDiffArr[j]) - 180)) << "\n";
                        //std::cout << fleeDeg << "fleeDeg between " << DangerAngleDiffArr[h] << " and " << DangerAngleDiffArr[j] << "\n";
                        fleeDeg = 180 - fabs(fabs(DangerAngleDiffArr[h] - DangerAngleDiffArr[j]) - 180);
                        fleeBound1 = DangerAngleDiffArr[h];
                        fleeBound2 = DangerAngleDiffArr[j];
                    }

            std::cout << fleeDeg << ": " << fleeBound1 << " to "<< fleeBound2 << "\n";

            fleeDeg = (fleeBound1 + fleeBound2) / 2;
            setCurrDestPoint(sin(fleeDeg / 180 * PI) * range + x, cos(fleeDeg / 180 * PI) * range + y);
        }
        else
        {
            setCurrDestPoint(x + (sin(DangerAngleDiffArr[0] / 180 * PI) * range), y + (cos(DangerAngleDiffArr[0] / 180 * PI) * range));
        }
        currDangerPoints.clear();
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

void AICity::addDangerPoint(std::string spriteName, sf::Sprite pSprite)
{
    currDangerPoints.insert(std::make_pair(spriteName, std::make_pair(pSprite.getPosition().x, pSprite.getPosition().y)));

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
        addDangerPoint(otherCity->name, otherCity->sprite);
    }
    else if (otherCity->size_ < size_ && currTargetName.empty()) // PURSUE IF NOT CURRENTLY PURSUING
    {
        currState = PURSUE; //Pursue
        currTargetName = otherCity->name;
        setCurrDestPoint(otherCity->sprite.getPosition().x, otherCity->sprite.getPosition().y);
    }
    else if (currState == PURSUE && otherCity->name.compare(currTargetName) == 0) //UPDATE currTargetPoint position of targetCity
    {
        setCurrDestPoint(otherCity->sprite.getPosition().x, otherCity->sprite.getPosition().y);
    }
}

void AICity::goToDestPoint()
{
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
            if (angle > targetAngle)
            {
                if (absDiff >= 180)
                    rotation(RIGHT);
                else if (absDiff < 180)
                    rotation(LEFT);
            }
            else
            {
                if (absDiff >= 180)
                    rotation(LEFT);
                else if (absDiff < 180)
                    rotation(RIGHT);
            }
        }
    }

    //angleDiff > 90
    if ((180 - fabs(fabs(angle - targetAngle) - 180)) < 90)
    {
        movement(FORWARD);
    }
    else if ((180 - fabs(fabs(angle - targetAngle) - 180)) > 90 )
    {
        movement(BACKWARD);
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

void AICity::setCurrDestPoint(double newX, double newY)
{
    if (newX < 0)
    {
        newX = 0;
    }
    else if (newY < 0)
    {
        newY = 0;
    }
    else if (newX > mapSize)
    {
            newX = mapSize;
    }
    else if (newY > mapSize)
    {
            newY = mapSize;
    }

    currDestPoint.first = newX;
    currDestPoint.second = newY;
}

//AICity END ------------------------------------------------------------
