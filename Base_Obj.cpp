#include "Base_Obj.h"

void AIObject::update(double x, double y)
{
    fovSprite.setPosition(x, y);
}

double AIObject::relAngle(double aiX, double aiY, double targetX, double targetY)
{
    double pointDiffX = aiX - targetX;
    double pointDiffY = aiY - targetY;

    double targetAngle = 1;

    targetAngle = atan(pointDiffX / pointDiffY) * 180 / PI;

    if (targetX > aiX && targetY > aiY)
        targetAngle -= 180;

    if (targetX < aiX && aiY < targetY)
        targetAngle += 180;

    return targetAngle;
}
