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

    City player_city (100, "Images/city.png");
    sf::View view; // could be used later to show view radius changes

    std::map<Point, TerrainTile> tileMap;
    Point point = Point(0, 0);
    TerrainTile tileM = TerrainTile(0, 0, "grass");
        std::cout << tileMap.size();

    tileMap.insert(std::pair<Point, TerrainTile>(point, tileM));
    std::cout << tileMap.size();

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
            player_city.move("forward");
        }
        else if (down_pressed)
        {
            player_city.move("backward");
        }

        if (left_pressed && (up_pressed ^ down_pressed))
        {
            player_city.rotate("left");
        }
        else if (right_pressed && (up_pressed ^ down_pressed))
        {
            player_city.rotate("right");
        }

        window.clear();
        player_city.update();
        world_map.update(player_city.mobileObj.x, player_city.mobileObj.y);
        window.draw(world_map.world);
        window.draw(player_city.sprite);
        window.display();
    }

    return 0;
}
