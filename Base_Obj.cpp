#include "Base_Obj.h"

void DangerPoint::addClosestPoint(DangerPoint * newClosestPoint, double distanceFrom)
{
    int posUpdated = -1;
    if (connectionCount == 2)
    {
        //If first point is closer than second point, replace second point OR if there is only one connection thus far
        if (closestPoints[0].second < closestPoints[1].second)
        {
            posUpdated = 1;
        }
        else
        {
            posUpdated = 0;
        }
    }
    else if (connectionCount == 1)
    {
        posUpdated = 1;
        connectionCount++;
    }
    else
    {
        posUpdated = 0;
        connectionCount++;
    }

    replaceClosestPoint(posUpdated, newClosestPoint, distanceFrom);
}

void DangerPoint::replaceClosestPoint(int pos, DangerPoint * newClosestPoint, double distanceFrom)
{
    closestPoints[pos].first = newClosestPoint;
    closestPoints[pos].second = distanceFrom;
}
