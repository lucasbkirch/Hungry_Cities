#include <SFML/Graphics.hpp>
#include <iostream>
#ifndef BASE_OBJ_H
#define BASE_OBJ_H
#include "Base_Obj.h"
#endif // BASE_OBJ_H
#include <list>

#define tileSideLength 20
#define numTerrainTypes 5

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
            std::cout << "Unrecognized Terrain Tile " << typeName << " at (" << x << ", " << y << ")\n";
        }
    }
};

class WorldMap
{
    public:

        sf::Sprite world;
        sf::Texture texture;
        std::map<std::pair<int, int>, TerrainTile *> tileMap;
        sf::Image mapImage;
        std::string terrainTypes[numTerrainTypes];
        sf::Color terrainColors[numTerrainTypes];
        std::list<TerrainTile *> collisionsCollection;
        std::map<std::string, sf::Color> terrainColorCollection;

        WorldMap(std::string mapFile) : terrainTypes {"grass", "snow", "rock", "dirt", "water"}, terrainColors {sf::Color(103, 189, 2), sf::Color(255, 255, 255), sf::Color(152, 150, 144), sf::Color(139, 101, 0), sf::Color(50, 17, 222)}
        {
            texture.loadFromFile(mapFile);
            world.setTexture(texture);
            mapImage = texture.copyToImage();

            for (int g = 0; g < numTerrainTypes; g++)
            {
                terrainColorCollection.insert(std::pair<std::string, sf::Color>(terrainTypes[g], terrainColors[g]));
            }

            InitializeTiles();
        }

        void InitializeTiles();
        std::list<TerrainTile *> terrainCollision(double, double, int, sf::Sprite);
};

