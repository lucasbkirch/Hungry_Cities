#include "WorldMap.h"

//TERRAINTILE BEGIN -----------------------------------------------

void TerrainTile::setTerrainType(std::string typeName)
{
    type = typeName;

    if (typeName.compare("grass") == 0)
    {
        speedModifier = 0.9;
    }
    else if (typeName.compare("dirt") == 0)
    {
        speedModifier = 1.0;
    }
    else if (typeName.compare("snow") == 0)
    {
        speedModifier = 0.75;
    }
    else if (typeName.compare("water") == 0)
    {
        speedModifier = 0;
    }
    else if (typeName.compare("rock") == 0)
    {
        speedModifier = 0.6;
    }
    else
    {
        std::cout << "Unrecognized Terrain Tile " << typeName << " at (" << x << ", " << y << ")\n";
    }
}

//TERRAINTILE END -------------------------------------------------
//WORLDMAP BEGIN --------------------------------------------------

void WorldMap::InitializeTiles()
{
    unsigned int i = 0;
    unsigned int j = 0;
    while (i < world.getGlobalBounds().width)
    {
        while (j < world.getGlobalBounds().height)
        {
            sf::Color color = mapImage.getPixel(i, j);
            std::string terrainTypeSelected;

            bool match = false;

            for (int h = 0; h < numTerrainTypes; h++)
            {
                if (color == terrainColorCollection[terrainTypes[h]])
                {
                    terrainTypeSelected = terrainTypes[h];
                    match = true;
                }
            }
            if (!match)
            {
                unsigned int rVal = color.r;
                unsigned int gVal = color.g;
                unsigned int bVal = color.b;
                std::cout << "Unrecognized color found (" << rVal << ", " << gVal << ", " << bVal << ")\n";
            }

            tileMap.insert(std::pair<std::pair<int, int>, TerrainTile *>(std::make_pair(i, j), new TerrainTile(i, j, terrainTypeSelected))); //TODO
            j += tileSideLength;
        }
        j = 0;
        i += tileSideLength;
    }
}

void WorldMap::CityGrassAffect(std::list<TerrainTile *> * grassCollisions)
{
    if (grassCollisions == NULL)
        return;

    std::list<TerrainTile *>::iterator grassIter;

    sf::Image dirtImage;
    dirtImage.loadFromFile("Images/dirt_terrain.png");
    for (grassIter = grassCollisions->begin(); grassIter != grassCollisions->end(); grassIter++)
    {
        (*grassIter)->setTerrainType("dirt");
        texture.update(dirtImage, (*grassIter)->x, (*grassIter)->y);
        delete (*grassIter);
    }
}

std::list<TerrainTile *> * WorldMap::terrainCollision(sf::Sprite sprite)
{
    //Find longest side
    double longestSide;
    if (sprite.getGlobalBounds().height > sprite.getGlobalBounds().width)
    {
        longestSide = sprite.getGlobalBounds().height;
    }
    else
    {
        longestSide = sprite.getGlobalBounds().width;
    }

    int bottomX = (sprite.getPosition().x - longestSide) / tileSideLength;
    bottomX *= tileSideLength;
    int topX = (sprite.getPosition().x + longestSide) / tileSideLength;
    topX *= tileSideLength;

    int bottomY = (sprite.getPosition().y - longestSide) / tileSideLength;
    bottomY *= tileSideLength;
    int topY = (sprite.getPosition().y + longestSide) / tileSideLength;
    topY *= tileSideLength;

    if (bottomX < 0)
        bottomX = 0;
    if (bottomY < 0)
        bottomY = 0;

    if (topX > mapSize)
        topX = mapSize;
    if (topY > mapSize)
        topY = mapSize;

    //Empty collisionsCollection in case of earlier use
    while (collisionsCollection.size() > 0)
    {
        collisionsCollection.pop_back();
    }

    for (int xCoord = bottomX; xCoord < topX; xCoord += tileSideLength)
        for (int yCoord = bottomY; yCoord < topY; yCoord += tileSideLength)
        {
            TerrainTile * targetTile = tileMap.at(std::make_pair(xCoord, yCoord));

            if (targetTile->tile.getGlobalBounds().intersects(sprite.getGlobalBounds()))
            {
                bool found = false;
                for (int v = 0; v < numTerrainTypes && !found; v++)
                {
                    if (targetTile->type.compare(terrainTypes[v]))
                    {
                        collisionsCollection.push_back(targetTile);
                        found = true;
                    }
                }
                if (!found)
                {
                    std::cout << "Terrain Type " << targetTile->type << " not recognized\n";
                }
            }
        }
    return &collisionsCollection;
}

//WORLDMAP END ----------------------------------------------------
