#include <SFML/Graphics.hpp>
#include <iostream>

#ifndef CITIES_H
#define CITIES_H
#include "Cities.h"
#endif // CITIES_H

#ifndef WORLDMAP_H
#define WORLDMAP_H
#include "WorldMap.h"
#endif // WORLDMAP_H

class HungryCitiesGame
{
    public:
        sf::RenderWindow window;
        WorldMap worldMap;
        std::map<std::string, Mobile_Object *> mobileObjDict;
        std::map<std::string, City *> cityDict;
        HungryCitiesGame(): window(sf::VideoMode(640, 640), "Hungry City Chronicles"), worldMap("Images/map_1.png")
        {
        }
        void updateAll(PlayerCity*);
        void drawAll();
        void cityCollisionCheck();
        void eventManagement();
        void keyPressManagement(PlayerCity*);
        void run();
};
