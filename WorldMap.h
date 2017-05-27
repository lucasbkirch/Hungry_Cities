#include <SFML/Graphics.hpp>
#include <iostream>
#ifndef BASE_OBJ_H
#define BASE_OBJ_H
#include "Base_Obj.h"
#endif // BASE_OBJ_H

class Point
{
    public:
        int x, y;
        Point(int xPos, int yPos)
        {
            x = xPos;
            y = yPos;
        }

        bool operator <(const Point& rhs) const
        {
            return !(rhs.x == x && rhs.y == y);
        }
};

class TerrainTile
{
public:
    sf::Sprite tile;
    int x, y;
    std::string type;
    double speedModifier;

    TerrainTile(int xPos, int yPos, std::string typeName)
    {
        x = xPos;
        y = yPos;
        setTerrainType(typeName);
        tile.setPosition(x, y);
        tile.setTextureRect(sf::IntRect(0, 0, 20, 20));
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
    std::map<Point, TerrainTile> tileMap;

    WorldMap(std::string mapFile)
    {
        texture.loadFromFile(mapFile);
        world.setTexture(texture);
    }
    void update(double, double);
    void InitializeTiles();
    std::map<std::string, int> terrainCollision(Mobile_Object, sf::FloatRect);
};

