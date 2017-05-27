#include <SFML/Graphics.hpp>
#include <iostream>
#include "Cities.h"
#include "WorldMap.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 640), "Hungry City Chronicles");
    sf::Image Icon;
    Icon.loadFromFile("Images/wagon_wheel_icon.png");
    window.setIcon(32, 32, Icon.getPixelsPtr());

    WorldMap world_map ("Images/map_1.png");
    world_map.InitializeTiles();

    std::map<std::string, City *> cityDict;
    std::map<std::string, City *> tempCityDict;

    PlayerCity playerCity ("London", 100, "Images/city.png");
    StaticCity targetCity ("Amsterdam", 50, "Images/circle.png");

    cityDict.insert(std::pair<std::string, City *>(playerCity.name, &playerCity));
    cityDict.insert(std::pair<std::string, City *>(targetCity.name, &targetCity));

    sf::View view; // could be used later to show view radius changes

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        bool up_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
        bool down_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
        bool left_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
        bool right_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);

        if (up_pressed)
        {
            playerCity.move("forward");
        }
        else if (down_pressed)
        {
            playerCity.move("backward");
        }

        if (left_pressed && (up_pressed ^ down_pressed))
        {
            playerCity.rotate("left");
        }
        else if (right_pressed && (up_pressed ^ down_pressed))
        {
            playerCity.rotate("right");
        }

        std::map<std::string, City *>::iterator cityIter;
        tempCityDict.insert(cityDict.begin(), cityDict.end());

        //City currCity;

        while(tempCityDict.size() > 0)
        {
            City currCity = *(tempCityDict.begin()->second);
            tempCityDict.erase(tempCityDict.begin()->first);

            for(cityIter = tempCityDict.begin(); cityIter != tempCityDict.end(); cityIter++)
            {
                if (currCity.sprite.getGlobalBounds().intersects((cityIter->second)->sprite.getGlobalBounds()))
                {
                    std::cout << "Collision between the cities " << currCity.name << " and " << cityIter->first << "\n";
                    //TODO Handle Collision!
                }
            }
        }
        window.clear();
        for(cityIter = cityDict.begin(); cityIter != cityDict.end(); cityIter++)
        {

            StaticCity* static_type = dynamic_cast<StaticCity*>(cityIter->second);

            if (static_type != NULL) //if Type StaticCity
            {
                static_type->update(playerCity);
                std::cout << "\nStatic City Updated " << static_type->name;
            }
            else //playerCity: Since there is only every 1 player city, there is no need to cast the currently selected cityIter->second to PlayerCity type and update that.
            {
                playerCity.update();
            }
        }
        world_map.update(playerCity.mobileObj.x, playerCity.mobileObj.y);
        window.draw(world_map.world);

        for(cityIter = cityDict.begin(); cityIter != cityDict.end(); cityIter++)
        {
            window.draw((cityIter->second)->sprite);
        }
        window.display();
    }

    return 0;
}
