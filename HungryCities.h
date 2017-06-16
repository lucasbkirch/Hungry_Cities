#include <SFML/Graphics.hpp>
#include <iostream>
#include <typeinfo>
#include <sstream>

#ifndef CITIES_H
#define CITIES_H
#include "Cities.h"
#endif // CITIES_H

#ifndef AI_H
#define AI_H
#include "AI.h"
#endif // AI_H

#define windowSize 640

class HungryCitiesGame
{
    private:
    public:
        //Variable Members
        sf::RenderWindow window;
        WorldMap worldMap;

        std::map<std::string, Mobile_Object *> mobileObjDict;
        std::map<std::string, City *> cityDict;
        std::list<sf::Sprite*> mapEdgeSprites;

        //Constructors
        HungryCitiesGame(): window(sf::VideoMode(windowSize, windowSize), "Hungry City Chronicles"), worldMap("Images/map_1.png")
        {
            sf::Image Icon;
            Icon.loadFromFile("Images/wagon_wheel_icon.png");
            window.setIcon(32, 32, Icon.getPixelsPtr());

            //Initialize mapEdgeSprites
            for (int i = 0; i <= mapSize; i += 100)
            {
                for (int j = 0; j <= mapSize; j += 100)
                {
                    if ((i == 0 || i == mapSize) || (j == 0 || j == mapSize))
                    {
                        sf::Sprite * newBorderSprite = new sf::Sprite();
                        newBorderSprite->setPosition(i, j);
                        newBorderSprite->setTextureRect(sf::IntRect(0, 0, 1, 1));
                        mapEdgeSprites.push_back(newBorderSprite);
                        std::cout << "Edge sprite placed at " << i << ", " << j << "\n";
                    }
                }
            }
        }

        //Methods
        void run();
        void drawAll();
        void cleanUp();
        void eventManagement();
        void cityCollisionCheck();
        void fovCollisionCheck();
        PlayerCity * cityInitialization(std::string, std::string);
};
