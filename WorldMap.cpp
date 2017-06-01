#include "WorldMap.h"
#include <list>

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
            currSize = tileMap.size();
            //std::cout << "Inserting: "<< i << ", " << j << "\n";
            tileMap.insert(std::pair<std::pair<int, int>, TerrainTile>(std::make_pair(i, j), TerrainTile(i, j, "grass"))); //TODO

            //std::cout << currSize << " " << tileMap.size() << "\n";
            if (currSize == tileMap.size())
            {
                std::cout << "Insertion Failed!\n";
            }
            else
            {
                //std::cout << "Retrieving: "<< i << ", " << j << "\n";
                //TerrainTile tt = tileMap[Point(i, j)];//This was adding an additional item to tileMap??????
                //TerrainTile tt = tileMap.at(Point(i, j)); //This gets an out of range error upon calling the (0, 20) point
            }
            j += 20;
        }
        j = 0;
        i += 20;
    }
    std::cout << tileMap.size() << " Tiles Initialized\n";
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
    {
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
//                        std::cout << collisionsCollection.at(targetTile.type) << "\n";
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    std::cout << "Terrain Type " << targetTile.type << " not recognized\n";
                }
            }

        }
    }
    return collisionsCollection;
}

void WorldMap::update(double player_x, double player_y)
{
    world.setPosition(0 - player_x, 0 - player_y);
}


/*
    double maxX = 0;
    double minX = 1000;

    double maxY = 0;
    double minY = 1000;

    double xPos[4];
    double yPos[4];

    xPos[0] = trans.transformPoint(0, 0).x; //(0, 0)
    xPos[1] = trans.transformPoint(rect.width, 0).x; //(50, 0)
    xPos[2] = trans.transformPoint(0, rect.height).x; //(0, 100)
    xPos[3] = trans.transformPoint(rect.width, rect.height).x; //(50, 100)

    yPos[0] = trans.transformPoint(0, 0).y; //(0, 0)
    yPos[1] = trans.transformPoint(rect.width, 0).y; //(50, 0)
    yPos[2] = trans.transformPoint(0, rect.height).y; //(0, 100)
    yPos[3] = trans.transformPoint(rect.width, rect.height).y; //(50, 100)

    for(int c = 0; c < 4; c++)
    {
        if (xPos[c] > maxX)
            maxX = xPos[c];
        if (xPos[c] < minX)
            minX = xPos[c];

        if (yPos[c] > maxY)
            maxY = yPos[c];
        if (yPos[c] < minY)
            minY = yPos[c];
    }

    std::cout << "miny: " << minY << "\n";
    std::cout << "maxy: " << maxY << "\n";
    std::cout << "minx: " << minX << "\n";
    std::cout << "maxx: " << maxX << "\n";*/
