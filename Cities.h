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

class City
{
    private:
    public:
        //Member Variables
        int size_;
        sf::Texture texture;
        sf::Sprite sprite;
        std::string name;

        //Constructors
        City(std::string, int, std::string);
        virtual ~City(){} //TODO -- https://stackoverflow.com/questions/15114093/getting-source-type-is-not-polymorphic-when-trying-to-use-dynamic-cast

        //Methods
        virtual std::list<TerrainTile *> * execute(std::list<TerrainTile *> *)
        {
            std::cout << "execute() is not implemented for this City\n";
            return NULL;
        };

};

class MobileCity: public City, public Mobile_Object
{
    private:
    public:
        //Members
        sf::Sprite wheelTracksSprite;

        //Constructors
        MobileCity(std::string, int, std::string);

        //Methods
        void rotate(std::string turn);
        void move(std::string direction);

        double calculateSpeedMod(double, std::string);

        std::list<TerrainTile *> * update(std::list<TerrainTile *> *);
        std::list<TerrainTile *> * terrainSpeedCalculation(std::list<TerrainTile *>*);
};

class PlayerCity: public MobileCity
{
    private:
    public:
        //Constructors
        PlayerCity(std::string, int, std::string);

        //Methods
        void keyPressManagement();
        std::list<TerrainTile *> * update(std::list<TerrainTile *> *);
        std::list<TerrainTile *> * execute(std::list<TerrainTile *> *) override;
};


class StaticCity: public City, public StaticObject
{
    private:
    public:
        //Constructors
        StaticCity(std::string, int, std::string);
};

class AICity: public MobileCity, public AIObject
{
    private:
    public:

        //Constructors
        AICity(double xPos, double yPos, unsigned int range) : MobileCity("AI CITY", 75, "Images/city.png"), AIObject(xPos, yPos, range)
        {
            //No Implementation, yet TODO?
        }

        //Methods
        std::list<TerrainTile *> * update(double, double, std::list<TerrainTile *> *);
};

