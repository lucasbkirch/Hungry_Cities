#include <SFML/Graphics.hpp>
#include <iostream>

#ifndef CITIES_H
#define CITIES_H
#include "Cities.h"
#endif // CITIES_H


#define WAIT_CYCLES 20

class HungryCitiesGame
{
    public:
        sf::RenderWindow window;
        WorldMap worldMap;
        std::map<std::string, Mobile_Object *> mobileObjDict;
        std::map<std::string, City *> cityDict;

        HungryCitiesGame(): window(sf::VideoMode(640, 640), "Hungry City Chronicles"), worldMap("Images/map_1.png")
        {
            sf::Image Icon;
            Icon.loadFromFile("Images/wagon_wheel_icon.png");
            window.setIcon(32, 32, Icon.getPixelsPtr());
        }
        void drawAll();
        void cityCollisionCheck();
        void eventManagement();
        //void keyPressManagement(PlayerCity*);
        void run();
        PlayerCity * cityInitialization(std::string, std::string);
        void cleanUp();
};
