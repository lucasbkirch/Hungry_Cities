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
        //allows view to center on the playerCity
        view.reset(sf::FloatRect(playerCity->x - (360 - 25), playerCity->y - (360 - 50), 640, 640));
        eventManagement();

        //std::list<TerrainTile *> * collisions = worldMap.terrainCollision(playerCity->sprite);

        //Replace with loop that calls execute for all cities
        //std::list<TerrainTile *> * grassCollisions = playerCity->execute(collisions);

        //worldMap.CityGrassAffect(grassCollisions);


        std::map<std::string, City *>::iterator cityIter;
        for(cityIter = cityDict.begin(); cityIter != cityDict.end(); cityIter++)
        {
            //First, it checks the terrain collisions with the city sprite -> returns terrainTile collection
            //Second, that city calls execute passing in the collection of terrain collisions -> returning a collection of 'grass' terrain tiles
            //Third, grass terrain tiles are switched to dirt and the worldMap image is updated to reflect this
            worldMap.CityGrassAffect(cityIter->second->execute(worldMap.terrainCollision(cityIter->second->sprite)));
        }

        cityCollisionCheck();

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
        PlayerCity* playerType = dynamic_cast<PlayerCity*>(cityIter->second);

        if (playerType == NULL) //Non-playercity
            window.draw((cityIter->second)->sprite);
        else
        {
            //an extra sprite was made for playerCity because a sprite is used for Collisions AND for drawing.
            //However, drawing for a player city is NOT attached to its curreny (x, y) as it is always in the center of the screen
            //BUT this conflicts with the drawing. Therefore, I made an additional sprite that is drawn for the playercity
            window.draw(playerType->sprite);
            //How to draw the playerCity sprite in the middle of the  screen without also affecting the location of the player
            //Cities Sprite? Make another sprite? One for drawing one for collision?
        }
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

PlayerCity * HungryCitiesGame::cityInitialization(std::string mobileCityImage, std::string staticCityImage)
{
    //Creating and adding all the city objects
    PlayerCity * playerCity = new PlayerCity("London",     100, mobileCityImage);
    StaticCity * targetCity1 = new StaticCity("Paris",     50,  staticCityImage);
    StaticCity * targetCity2 = new StaticCity("Berlin",    50,  staticCityImage);
    StaticCity * targetCity3 = new StaticCity("Kalingrad", 50,  staticCityImage);
    StaticCity * targetCity4 = new StaticCity("Amsterdam", 50,  staticCityImage);

    //Adding to cityDict all mobile and static cities
    cityDict.insert(std::pair<std::string, City *>(playerCity->name, playerCity));

    cityDict.insert(std::pair<std::string, City *>(targetCity1->name, targetCity1));
    cityDict.insert(std::pair<std::string, City *>(targetCity2->name, targetCity2));
    cityDict.insert(std::pair<std::string, City *>(targetCity3->name, targetCity3));
    cityDict.insert(std::pair<std::string, City *>(targetCity4->name, targetCity4));

    return playerCity;
}
