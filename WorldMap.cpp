#include "WorldMap.h"

void WorldMap::InitializeTiles()
{
    int i = 0;
    int j = 0;
    while (i < world.getGlobalBounds().width)
    {
        while (j < world.getGlobalBounds().height)
        {
            tileMap.insert(std::pair<Point, TerrainTile>(Point(i, j), TerrainTile(i, j, "grass"))); //TODO
            j += 20;
        }
        j = 0;
        i += 20;
    }
    std::cout << "Number of Tiles " << tileMap.size();

}

void WorldMap::update(double player_x, double player_y)
{
    world.setPosition(0 - player_x, 0 - player_y);
}
