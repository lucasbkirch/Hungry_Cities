#include "WorldMap.h"

void WorldMap::InitializeTiles()
{
    int i = 0;
    int j = 0;
    while (i < world.getGlobalBounds().width)
    {
        while (j < world.getGlobalBounds().height)
        {
            if (i == 0 && j == 0)
            {
                std::cout << "0, 0\n";
                TerrainTile * tt = new TerrainTile(i, j, "grass");
                sf::Texture texture;
                texture.loadFromFile("Images/circle.png");
                tt->tile.setTexture(texture);
            }
            else
                tileMap.insert(std::pair<std::pair<int, int>, TerrainTile *>(std::make_pair(i, j), new TerrainTile(i, j, "grass"))); //TODO
            j += tileSideLength;
        }
        j = 0;
        i += tileSideLength;
    }
}

std::map<std::string, int> WorldMap::terrainCollision(double x, double y, int largestSide, sf::Sprite sprite)
{
    int bottomX = (x - largestSide / 2) / tileSideLength;
    bottomX *= tileSideLength;
    int topX = (x + largestSide / 2) / tileSideLength;
    topX *= tileSideLength;

    int bottomY = (y - largestSide / 2) / tileSideLength;
    bottomY *= tileSideLength;
    int topY = (y + largestSide / 2) / tileSideLength;
    topY *= tileSideLength;

    for (int f = 0; f < terrainTypesLength; f++)
    {
        collisionsCollection[terrainTypes[f]] = 0;
    }

    for (int xCoord = bottomX; xCoord < topX; xCoord += tileSideLength)
        for (int yCoord = bottomY; yCoord < topY; yCoord += tileSideLength)
        {
            TerrainTile * targetTile = tileMap.at(std::make_pair(xCoord, yCoord));

            if (targetTile->tile.getGlobalBounds().intersects(sprite.getGlobalBounds()))
            {
                bool found = false;
                for (int v = 0; v < terrainTypesLength && !found; v++)
                {
                    if (targetTile->type.compare(terrainTypes[v]))
                    {
                        collisionsCollection[targetTile->type]++;
                        found = true;
                    }
                }
                if (!found)
                    std::cout << "Terrain Type " << targetTile->type << " not recognized\n";
            }
        }
    return collisionsCollection;
}

void WorldMap::update(double player_x, double player_y)
{
    world.setPosition(0 - player_x, 0 - player_y);
}
