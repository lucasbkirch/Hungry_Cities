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
    private:
    public:

        //Member Variables
        int x, y;
        double speedModifier;
        std::string type;
        sf::Sprite tile;
        sf::Texture texture;

        //Constructors
        TerrainTile(int xPos, int yPos, std::string typeName)
        {
            x = xPos;
            y = yPos;
            setTerrainType(typeName);
            tile.setTextureRect(sf::IntRect(0, 0, tileSideLength, tileSideLength));
            tile.setPosition(x, y);
        }

        //Methods
        void setTerrainType(std::string);
};

class WorldMap
{
    public:

        //Member Variables
        sf::Sprite world;
        sf::Texture texture;
        sf::Image mapImage;

        std::string terrainTypes[numTerrainTypes];
        sf::Color terrainColors[numTerrainTypes];

        std::map<std::pair<int, int>, TerrainTile *> tileMap;
        std::map<std::string, sf::Color> terrainColorCollection;

        std::list<TerrainTile *> collisionsCollection;

        //Constructors
        WorldMap(std::string mapFile) :
            terrainTypes {"grass", "snow", "rock", "dirt", "water"},
            terrainColors {sf::Color(103, 189, 2), sf::Color(255, 255, 255), sf::Color(152, 150, 144), sf::Color(139, 101, 0), sf::Color(50, 17, 222)}
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

        //Methods
        void InitializeTiles();
        void CityGrassAffect(std::list<TerrainTile *> *);
        std::list<TerrainTile *> * terrainCollision(sf::Sprite);
};

