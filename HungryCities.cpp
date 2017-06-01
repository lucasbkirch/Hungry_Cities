#include "HungryCities.h"

void HungryCitiesGame::run()
{
    sf::Image Icon;
    Icon.loadFromFile("Images/wagon_wheel_icon.png");
    window.setIcon(32, 32, Icon.getPixelsPtr());

    //Creating and adding all the city objects
    PlayerCity playerCity ("London", 100, "Images/city.png");
    StaticCity targetCity1 ("Amsterdam", 50, "Images/circle.png");
    StaticCity targetCity2 ("Paris", 50, "Images/circle.png");
    StaticCity targetCity3 ("Berlin", 50, "Images/circle.png");
    StaticCity targetCity4 ("Kalingrad", 50, "Images/circle.png");

    //Adding to cityDict all mobile and static cities
    cityDict.insert(std::pair<std::string, City *>(playerCity.name, &playerCity));
    cityDict.insert(std::pair<std::string, City *>(targetCity1.name, &targetCity1));
    cityDict.insert(std::pair<std::string, City *>(targetCity2.name, &targetCity2));
    cityDict.insert(std::pair<std::string, City *>(targetCity3.name, &targetCity3));
    cityDict.insert(std::pair<std::string, City *>(targetCity4.name, &targetCity4));

    //sf::View view; // TODO could be used later to show view radius changes
    int terrainCheckTimer = 0;
    while (window.isOpen())
    {
        eventManagement();
        keyPressManagement(&playerCity);
        cityCollisionCheck();

        if (terrainCheckTimer >= 20)
        {
            worldMap.terrainCollision(playerCity.x, playerCity.y, 100, playerCity.sprite);
            terrainCheckTimer = 0;
        }
        else
            terrainCheckTimer++;

        updateAll(&playerCity);
        drawAll();
    }

}

void HungryCitiesGame::eventManagement()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void HungryCitiesGame::keyPressManagement(PlayerCity * playerCity)
{
    bool up_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    bool down_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    bool left_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    bool right_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);

    if (up_pressed)
    {
        playerCity->move("forward");
    }
    else if (down_pressed)
    {
        playerCity->move("backward");
    }

    if (left_pressed && (up_pressed ^ down_pressed))
    {
        playerCity->rotate("left");
    }
    else if (right_pressed && (up_pressed ^ down_pressed))
    {
        playerCity->rotate("right");
    }
}

void HungryCitiesGame::cityCollisionCheck()
{
    //Create Temp dictionary so that the current city won't be asked if it
    //intersects itself by removing it from the temp dictionary prior to
    //comparison with all other cities in the temp dictionary
    std::map<std::string, City *>::iterator cityIter;
    std::map<std::string, City *> tempCityDict;

    tempCityDict.insert(cityDict.begin(), cityDict.end());

    //Checking for collisions between any cities
    while(tempCityDict.size() > 0)
    {
        City currCity = *(tempCityDict.begin()->second);
        tempCityDict.erase(tempCityDict.begin()->first);

        for(cityIter = tempCityDict.begin(); cityIter != tempCityDict.end(); cityIter++)
        {
            if (currCity.sprite.getGlobalBounds().intersects((cityIter->second)->sprite.getGlobalBounds()))
            {
                std::cout << "Collision between the cities " << currCity.name << currCity.size_ << " and " << cityIter->first << (cityIter->second)->size_  << "\n";
                //TODO Handle Collision!
                if (currCity.size_ >= (cityIter->second)->size_)
                {
                    cityDict.erase((cityIter->second)->name);
                }
                else
                {
                    cityDict.erase(currCity.name);
                }
            }
        }
    }
}

void HungryCitiesGame::drawAll()
{
    std::map<std::string, City *>::iterator cityIter;
    window.clear();

    //Draw the map, cities and other objects in the world
    window.draw(worldMap.world);

    for(cityIter = cityDict.begin(); cityIter != cityDict.end(); cityIter++)
    {
        window.draw((cityIter->second)->sprite);
    }
    window.display();

}

void HungryCitiesGame::updateAll(PlayerCity * playerCity)
{
    std::map<std::string, City *>::iterator cityIter;

        //Update all cities
    for(cityIter = cityDict.begin(); cityIter != cityDict.end(); cityIter++)
    {

        StaticCity* static_type = dynamic_cast<StaticCity*>(cityIter->second);

        if (static_type != NULL) //if Type StaticCity
        {
            static_type->update(*playerCity);
        }
        else //playerCity: Since there is only every 1 player city, there is no need to cast the currently selected cityIter->second to PlayerCity type and update that.
        {
            playerCity->update();
        }
    }
    worldMap.update(playerCity->x, playerCity->y);
}
