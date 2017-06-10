#include "Base_Obj.h"

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
