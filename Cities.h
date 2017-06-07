#include <SFML/Graphics.hpp>
#include <iostream>

#ifndef BASE_OBJ_H
#define BASE_OBJ_H
#include "Base_Obj.h"
#endif // BASE_OBJ_H

#ifndef WORLDMAP_H
#define WORLDMAP_H
#include "WorldMap.h"
#endif // WORLDMAP_H

#include <list>

#include <cmath>
#include <iostream>
#define PI 3.14159265
#include <time.h>

//class TerrainTile;

class City
{
    public:
        int size_;
        sf::Texture texture;
        sf::Sprite sprite;
        std::string name;
        City(std::string, int, std::string);
        virtual ~City(){} //TODO -- https://stackoverflow.com/questions/15114093/getting-source-type-is-not-polymorphic-when-trying-to-use-dynamic-cast
        virtual std::list<TerrainTile *> * execute(std::list<TerrainTile *> *)
        {
            std::cout << "execute() is not implemented for this City\n";
            return NULL;
        };

};

class MobileCity: public City, public Mobile_Object
{
    public:
        sf::Sprite wheelTracksSprite;
        MobileCity(std::string, int, std::string);
        void rotate(std::string turn);
        void move(std::string direction);
        std::list<TerrainTile *> * update(std::list<TerrainTile *> *);
        double calculateSpeedMod(double, std::string);
        std::list<TerrainTile *> * terrainSpeedCalculation(std::list<TerrainTile *>*);
};

class PlayerCity: public MobileCity
{

    public:
        PlayerCity(std::string, int, std::string);
        std::list<TerrainTile *> * update(std::list<TerrainTile *> *);
        void keyPressManagement();
        std::list<TerrainTile *> * execute(std::list<TerrainTile *> *) override;
};


class StaticCity: public City, public StaticObject
{
    public:
        StaticCity(std::string, int, std::string);
};

class AICity: public MobileCity, public AIObject
{
    public:
        void move();
        AICity(double xPos, double yPos, unsigned int range) : MobileCity("AI CITY", 75, "Images/city.png"), AIObject(xPos, yPos, range)
        {}
        std::list<TerrainTile *> * update(double, double, std::list<TerrainTile *> *);

};

