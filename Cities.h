#include <SFML/Graphics.hpp>
#include <iostream>
#include "Base_Obj.h"

class City
{
    public:
        int size;
        sf::Texture texture;
        sf::Sprite sprite;
        std::string name;
        City(std::string, int, std::string);
        virtual ~City(){} //TODO -- https://stackoverflow.com/questions/15114093/getting-source-type-is-not-polymorphic-when-trying-to-use-dynamic-cast
};

class MobileCity: public City
{
    public:

        MobileCity(std::string, int, std::string);
        Mobile_Object mobileObj;
        void rotate(std::string turn);
        void move(std::string direction);
        void update();
};

class PlayerCity: public MobileCity
{
    public:
        PlayerCity(std::string, int, std::string);
};

class StaticCity: public City
{
    public:
        int x, y;
        StaticCity(std::string, int, std::string);
        void update(PlayerCity);
};

