#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "Actor.h"
#include "GameWorld.h"

#include <string>
#include <queue>
#include <vector>

class StudentWorld : public GameWorld
{
    public:
        StudentWorld(std::string assetPath);
        ~StudentWorld();
        virtual int init();
        virtual int move();
        virtual void cleanUp();
        void addActor(Actor* a);
        bool hitCheck(double x, double y, double radius, Actor* orig);
        bool socCheck(double x, double y, double radius);
        void hurtSoc(int amt);
        void healSoc();
        void addCharges(int amt);
    private:
        std::vector<Actor*> m_actors;
        std::queue<Actor*> checkOverlap(double x, double y, double radius, Actor* orig);
        template<class A> void populate(int num);
        
        Socrates* m_socrates;
        int m_level;
        int m_nBaddies;
};

#endif // STUDENTWORLD_H_
