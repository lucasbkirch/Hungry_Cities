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
    double pointDiffX = aiPos.first - targetPos.first;
    double pointDiffY = aiPos.second - targetPos.second;

    double targetAngle = 1;

    targetAngle = atan(pointDiffX / pointDiffY) * 180 / PI;

    if (targetPos.first > aiPos.first && targetPos.second > aiPos.second)
        targetAngle -= 180;

    if (targetPos.first < aiPos.first && aiPos.second < targetPos.second)
        targetAngle += 180;

    return targetAngle;
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
    }
}

void AICity::flee()
{
    if ((abs(currDestPoint.first - x) < 50 && abs(currDestPoint.second - y) < 50) || currDangerPoints.size() < 1) // If point is reached, wander
    {
        currState = IDLE;
        currTargetName = "";
        setCurrDestPoint(0, 0);
    }

    calcFleePoint();
    goToDestPoint();
}

void AICity::calcFleePoint()
{
        std::map<std::string, std::pair<double, double>>::iterator dangerIter;
        double totalDeg = 0;
        for (dangerIter = currDangerPoints.begin(); dangerIter != currDangerPoints.end(); dangerIter++)
        {
            totalDeg += calcRelativeAngle(std::make_pair(x, y), dangerIter->second);
        }

        //inverse the angle
        double fleeDeg = totalDeg / currDangerPoints.size();

        setCurrDestPoint(sin(-fleeDeg) * range + x, cos(-fleeDeg) * range + y);

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

        currDangerPoints.insert(std::make_pair(otherCity->name, std::make_pair(otherCity->sprite.getPosition().x, otherCity->sprite.getPosition().y)));
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
    else if (newX > screenSize)
    {
            newX = screenSize;
    }
    else if (newY > screenSize)
    {
            newY = screenSize;
    }

    currDestPoint.first = newX;
    currDestPoint.second = newY;
}

//AICity END ------------------------------------------------------------
