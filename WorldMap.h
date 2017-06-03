#include <SFML/Graphics.hpp>
#include <iostream>
#ifndef BASE_OBJ_H
#define BASE_OBJ_H
#include "Base_Obj.h"
#endif // BASE_OBJ_H

#define tileSideLength 20

class TerrainTile
{
public:
    sf::Sprite tile;
    int x, y;
    std::string type;
    sf::Texture texture;
    double speedModifier;

    TerrainTile(int xPos, int yPos, std::string typeName)
    {
        x = xPos;
        y = yPos;
        setTerrainType(typeName);
        tile.setTextureRect(sf::IntRect(0, 0, tileSideLength, tileSideLength));
        tile.setPosition(x, y);
    }

    void setTerrainType(std::string typeName)
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
            std::cout << "Unrecognized Terrain Tile " << typeName << " at (" << x << ", " << y << ")";
        }
    }
};

class WorldMap
{
    public:
        sf::Sprite world;
        sf::Texture texture;
        std::map<std::pair<int, int>, TerrainTile *> tileMap;
        int terrainTypesLength;

        std::string terrainTypes[5];
        std::map<std::string, int> collisionsCollection;


        WorldMap(std::string mapFile) : terrainTypes{"grass", "snow", "rock", "dirt", "water"}
        {
            texture.loadFromFile(mapFile);
            world.setTexture(texture);
            InitializeTiles();

            terrainTypesLength = 5;
            for (int g = 0; g < terrainTypesLength; g++)
                collisionsCollection.insert(std::pair<std::string, int>(terrainTypes[g], 0));
        }

        void update(double, double);
        void InitializeTiles();
        std::map<std::string, int> terrainCollision(double, double, int, sf::Sprite);
};

