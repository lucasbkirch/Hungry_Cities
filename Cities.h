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
#include <time.h>

#define PI 3.14159265

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

        City(std::string cityName, int sz, std::string txturName)
        {
                size_ = sz;
                name = cityName;
                texture.loadFromFile(txturName);
                sprite.setTexture(texture);
        }
        virtual ~City(){} //TODO -- https://stackoverflow.com/questions/15114093/getting-source-type-is-not-polymorphic-when-trying-to-use-dynamic-cast

        //Methods
        virtual std::list<TerrainTile *> * execute(std::list<TerrainTile *> *)
        {
            std::cout << "execute() is not implemented for this City\n";
            return NULL;
        };

        virtual std::string collideWith(City * collisionCity)
        {
            std::cout << collisionCity->name << ": " << collisionCity->size_ << " vs. " << name << ": " << size_ << "\n";
            if (collisionCity->size_ > size_)
                return name;
            else if (collisionCity->size_ < size_)
                return collisionCity->name;
            else
                return ""; //empty string
        };
};

class StaticCity: public City, public StaticObject
{
    private:
    public:
        //Constructors
        StaticCity(std::string cityName, int sz, std::string txturName)
        : City(cityName, sz, txturName)
        {
            x = rand() % screenSizeSpawnArea + 250;
            y = rand() % screenSizeSpawnArea + 250;
            std::cout << "Placed at (" << x << ", " << y << ")\n";
            sprite.setPosition(x, y);
            sprite.setTextureRect(sf::IntRect(0, 0, 100, 100));
            sprite.setOrigin(25, 50);
        }
        std::list<TerrainTile *> * execute(std::list<TerrainTile *> *) override;
};

class MobileCity: public City, public Mobile_Object
{
    private:
    public:
        //Members
        sf::Sprite wheelTracksSprite;

        //Constructors
        MobileCity(std::string cityName, int sz, std::string txturName)
        : City(cityName, sz, txturName)
        {
            sprite.setTextureRect(sf::IntRect(0, 0, 50, 100));
            wheelTracksSprite.setTextureRect(sf::IntRect(0, 80, 10, 20));
            sprite.setOrigin(25, 50);
        }

        //Methods
        void move(std::string direction);
        void rotate(std::string turn);

        double calculateSpeedMod(double, std::string);

        std::list<TerrainTile *> * update(std::list<TerrainTile *> *);
        std::list<TerrainTile *> * terrainSpeedCalculation(std::list<TerrainTile *>*);
};

class PlayerCity: public MobileCity
{
    private:
    public:
        //Constructors
        PlayerCity(std::string cityName, int sz, std::string txtureName): MobileCity(cityName, sz, txtureName)
        {
            x = 2500;
            y = 2500;
            sprite.setPosition(x, y);
        }

        //Methods
        void keyPressManagement();
        std::list<TerrainTile *> * update(std::list<TerrainTile *> *);
        std::list<TerrainTile *> * execute(std::list<TerrainTile *> *) override;
};

class AICity: public MobileCity, public AIObject
{
    private:
    public:
        //Member Variables
        int currState;
        std::string currTargetName;
        std::pair<int, int> currDestPoint;
        std::map<std::string, std::pair<double, double>> currDangerPoints;

        //Constructors
        AICity(std::string cityName, int sz, std::string imageName, unsigned int range) : MobileCity(cityName, sz, imageName), AIObject(x, y, range)
        {
            //No Implementation, yet TODO?
            currState = 3;
            currDestPoint = std::make_pair(-1, -1);
        }

        AICity(std::string cityName, int sz, std::string imageName, unsigned int range, double xPos, double yPos) : MobileCity(cityName, sz, imageName), AIObject(xPos, yPos, range)
        {
            //No Implementation, yet TODO?
            x = xPos;
            y = yPos;
            currState = 3;
            currDestPoint = std::make_pair(-1, -1);
        }

        //Methods
        void behaviorManagement();
        void pursue();
        void flee();
        void idle();
        void wander();
        void updateCurrState(City *);
        void goToDestPoint();
        std::list<TerrainTile *> * update(std::list<TerrainTile *> *);
        std::list<TerrainTile *> * execute(std::list<TerrainTile *> *) override;
};

