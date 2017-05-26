#include <SFML/Graphics.hpp>
#include <iostream>
#include "Base_Obj.h"

class City
{
    int size;
    sf::Texture texture;
    public:
        Mobile_Object mobileObj;
        sf::Sprite sprite;
        City(int, std::string);
        void rotate(std::string turn);
        void move(std::string direction);
        void update();
};
