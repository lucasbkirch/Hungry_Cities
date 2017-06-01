#include "WorldMap.h"

int terrainTypesLength = 5;
std::string terrainTypes[] = {"grass", "snow", "rock", "dirt", "water"};

void WorldMap::InitializeTiles()
{
    int currSize = 0;
    int i = 0;
    int j = 0;
    while (i < world.getGlobalBounds().width)
    {
        while (j < world.getGlobalBounds().height)
        {
            tileMap.insert(std::pair<std::pair<int, int>, TerrainTile>(std::make_pair(i, j), TerrainTile(i, j, "grass"))); //TODO
            j += 20;
        }
        j = 0;
        i += 20;
    }
}

std::map<std::string, int> WorldMap::terrainCollision(double x, double y, int largestSide, sf::Sprite sprite)
{
    std::map<std::string, int> collisionsCollection;

    for (int g = 0; g < terrainTypesLength; g++)
        collisionsCollection.insert(std::pair<std::string, int>(terrainTypes[g], 0));

    int bottomX = (x - largestSide / 2) / 20;
    bottomX *= 20;
    int topX = (x + largestSide / 2) / 20;
    topX *= 20;

    int bottomY = (y - largestSide / 2) / 20;
    bottomY *= 20;
    int topY = (y + largestSide / 2) / 20;
    topY *= 20;

    for (int xCoord = 0; xCoord < topX; xCoord += 20)
        for (int yCoord = 0; yCoord < topY; yCoord += 20)
        {
            TerrainTile targetTile = tileMap.at(std::make_pair(xCoord, yCoord));
            if (targetTile.tile.getGlobalBounds().intersects(sprite.getGlobalBounds()))
            {
                bool found = false;
                for (int v = 0; v < terrainTypesLength && !found; v++)
                {
                    if (targetTile.type.compare(terrainTypes[v]))
                    {
                        collisionsCollection.at(targetTile.type)++;
                        found = true;
                    }
                }
                if (!found)
                    std::cout << "Terrain Type " << targetTile.type << " not recognized\n";
            }
        }

    return collisionsCollection;
}

void WorldMap::update(double player_x, double player_y)
{
    world.setPosition(0 - player_x, 0 - player_y);
}
