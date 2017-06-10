#include <SFML/Graphics.hpp>
#include <iostream>
#define screenSizeSpawnArea 4500
#define screenSize 5000

#define PI 3.14159265
#define PURSUE 1
#define FLEE 2
#define IDLE 3
#define WANDER 4

#define FORWARD -1
#define BACKWARD 1

#define LEFT 1
#define RIGHT -1

class StaticObject
{
    public:
        //Members
        int x, y;
};

class Mobile_Object
{
    public:
        //Member Variables
        double baseMoveSpd, moveSpd, angle, turn_rate, x, y;

        //Constructors
        Mobile_Object(int x_, int y_)
        {
            x = x_;
            y = y_;
            angle = 0;
            turn_rate = 0.1;
            baseMoveSpd = 0.00025;
            moveSpd = baseMoveSpd;
        }

        Mobile_Object()
        {
            x = rand() % screenSizeSpawnArea + 250;
            y = rand() % screenSizeSpawnArea + 250;
            angle = 0;
            turn_rate = 0.1;
            baseMoveSpd = 0.25;
            moveSpd = baseMoveSpd;
        }

        Mobile_Object(int start_x, int start_y, double b_move_spd, double trn_rate)
        {
            x = start_x;
            y = start_y;
            baseMoveSpd = b_move_spd;
            turn_rate = trn_rate;
            moveSpd = baseMoveSpd;
        }
};

class AIObject
{
    public:
        //Member Variables
        sf::Sprite fovSprite;
        sf::Texture texture;

        //Constructors
        AIObject(double x, double y, unsigned int range)
        {
            fovSprite.setPosition(x, y);
            fovSprite.setTextureRect(sf::IntRect(0, 0, range, range));
            fovSprite.setOrigin(range/2, range/2);
            texture.loadFromFile("Images/fovSprite.png");
            fovSprite.setTexture(texture);
        }

        //Methods
        void update(double, double);
        double relAngle(double, double, double, double);
};
