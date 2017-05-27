#include <SFML/Graphics.hpp>
#include <iostream>


class StaticObject
{
    public:
        int x, y;
};

class Mobile_Object
{
    public:
        double baseMoveSpd, moveSpd, angle, turn_rate, x, y;

        Mobile_Object()
        {
            x = 2500; //TODO
            y = 2500; // TODO get actual map size
            angle = 0;
            turn_rate = 0.05;
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

class AI_Object
{


};
