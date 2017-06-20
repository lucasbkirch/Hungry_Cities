#ifndef BASE_OBJ_H
#define BASE_OBJ_H
#include "Base_Obj.h"
#endif // BASE_OBJ_H

#ifndef CITIES_H
#define CITIES_H
#include "Cities.h"
#endif // CITIES_H

class AIObject
{
    public:
        //Member Variables
        sf::Sprite fovSprite;
        sf::Texture texture;
        unsigned int range;

        //Constructors
        AIObject(double x, double y, unsigned int range_)
        {
            range = range_;
            fovSprite.setOrigin(range/2, range/2);

            fovSprite.setPosition(x, y);
            fovSprite.setTextureRect(sf::IntRect(0, 0, range, range));
            texture.loadFromFile("Images/fovSprite.png");
            fovSprite.setTexture(texture);
        }

        //Methods
        void update(double, double);
        double calcRelativeAngle(std::pair<double, double>, std::pair<double, double>);
        double inverseAngle(double);
};


class AICity: public MobileCity, public AIObject
{
    private:
        std::pair<double, double> currDestPoint;

    public:
        //Member Variables
        int currState;
        bool movingForward;
        std::string currTargetName;
        std::list<DangerPoint *> currDangerPoints;

        //Constructors
        AICity(std::string cityName, int sz, std::string imageName, unsigned int range) : MobileCity(cityName, sz, imageName), AIObject(x, y, range)
        {
            //No Implementation, yet TODO?
            currState = IDLE;
            currDestPoint = std::make_pair(-1, -1);
        }

        AICity(std::string cityName, int sz, std::string imageName, unsigned int range, double xPos, double yPos) : MobileCity(cityName, sz, imageName), AIObject(xPos, yPos, range)
        {
            //No Implementation, yet TODO?
            x = xPos;
            y = yPos;
            currState = IDLE;
            currDestPoint = std::make_pair(-1, -1);
            movingForward = false;
        }

        //Methods
        void behaviorManagement();
        void pursue();
        void flee();
        void idle();
        void wander();
        void updateCurrState(City *);
        void goToDestPoint();
        void calcFleePoint();
        void setCurrDestPoint(double, double);
        void addDangerPoint(sf::Sprite);
        std::list<TerrainTile *> * update(std::list<TerrainTile *> *);
        std::list<TerrainTile *> * execute(std::list<TerrainTile *> *) override;
};
