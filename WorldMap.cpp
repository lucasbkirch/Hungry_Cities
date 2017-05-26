#include "WorldMap.h"

void WorldMap::InitializeTiles()
{
    int i = 0;
    int j = 0;
    while (i < world.getGlobalBounds().width)
    {
        while (j < world.getGlobalBounds().height)
        {
            j += 20;
        }
        j = 0;
        i += 20;
    }

}

void WorldMap::update(double player_x, double player_y)
{
    world.setPosition(0 - player_x, 0 - player_y);
}
