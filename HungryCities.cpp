#include "HungryCities.h"

void HungryCitiesGame::run()
{
    srand(time(NULL));

    //Creating and adding all the city objects
    PlayerCity * playerCity = cityInitialization("Images/city.png", "Images/circle.png");

    sf::View view; // TODO could be used later to show view radius changes

    //Game Loop
    while (window.isOpen())
    {
        //View to center on the playerCity
        view.reset(sf::FloatRect(playerCity->x - (360 - 25), playerCity->y - (360 - 50), 640, 640));
        eventManagement();

        std::map<std::string, City *>::iterator cityIter;
        for(cityIter = cityDict.begin(); cityIter != cityDict.end(); cityIter++)
        {
            //First, it checks the terrain collisions with the city sprite -> returns terrainTile collection
            //Second, that city calls execute passing in the collection of terrain collisions -> returning a collection of 'grass' terrain tiles
            //Third, grass terrain tiles are switched to dirt and the worldMap image is updated to reflect this
            worldMap.CityGrassAffect(cityIter->second->execute(worldMap.terrainCollision(cityIter->second->sprite)));
        }

        cityCollisionCheck();
        fovCollisionCheck();
        drawAll();
        window.setView(view);
    }

    cleanUp();
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

void HungryCitiesGame::cleanUp()
{
    std::cout << "Cleanup\n";
    std::map<std::string, City *>::iterator cityIter;
    for(cityIter = cityDict.begin(); cityIter != cityDict.end(); cityIter++)
    {
        std::cout << "deallocating " << cityIter->second->name << "\n";
        delete cityIter->second;
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
            if (currCity.sprite.getGlobalBounds().intersects((cityIter->second)->sprite.getGlobalBounds()))
            {
                std::string destroyedCity = currCity.collideWith(cityIter->second);
                if (!destroyedCity.empty())
                    cityDict.erase(destroyedCity);

            }


    }
}

void HungryCitiesGame::fovCollisionCheck()
{
    //Create Temp dictionary so that the current city won't be asked if it
    //intersects itself by removing it from the temp dictionary prior to
    //comparison with all other cities in the temp dictionary
    std::map<std::string, City *>::iterator seeingCityIter;
    std::map<std::string, City *>::iterator seenCityIter;

    for (seeingCityIter = cityDict.begin(); seeingCityIter != cityDict.end(); seeingCityIter++)
    {
        AICity * ai = dynamic_cast<AICity *>((seeingCityIter->second));
        if (ai != NULL)
            for(seenCityIter = cityDict.begin(); seenCityIter != cityDict.end(); seenCityIter++)
                if (seeingCityIter->second->name.compare(seenCityIter->second->name) != 0 && ai->fovSprite.getGlobalBounds().intersects((seenCityIter->second)->sprite.getGlobalBounds()))
                    ai->updateCurrState(seenCityIter->second);
    }
}

PlayerCity * HungryCitiesGame::cityInitialization(std::string mobileCityImage, std::string staticCityImage)
{
    //Creating and adding all the city objects
    PlayerCity * playerCity = new PlayerCity("London",     100, mobileCityImage);
    StaticCity * targetCity1 = new StaticCity("Paris",     50,  staticCityImage);
    StaticCity * targetCity2 = new StaticCity("Berlin",    50,  staticCityImage);
    StaticCity * targetCity3 = new StaticCity("Kalingrad", 50,  staticCityImage);
    StaticCity * targetCity4 = new StaticCity("Amsterdam", 50,  staticCityImage);
    AICity * targetCity5 = new AICity("Munich", 110,  mobileCityImage, 640, 2000, 2000);

    //Adding to cityDict all mobile and static cities
    cityDict.insert(std::pair<std::string, City *>(playerCity->name, playerCity));

    cityDict.insert(std::pair<std::string, City *>(targetCity1->name, targetCity1));
    cityDict.insert(std::pair<std::string, City *>(targetCity2->name, targetCity2));
    cityDict.insert(std::pair<std::string, City *>(targetCity3->name, targetCity3));
    cityDict.insert(std::pair<std::string, City *>(targetCity4->name, targetCity4));
    cityDict.insert(std::pair<std::string, City *>(targetCity5->name, targetCity5));


    return playerCity;
}
