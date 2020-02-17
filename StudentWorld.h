#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "Actor.h"
#include "GameWorld.h"

#include <string>
#include <vector>

class StudentWorld : public GameWorld
{
    public:
        StudentWorld(std::string assetPath);
        ~StudentWorld();
        virtual int init();
        virtual int move();
        virtual void cleanUp();
    private:
        std::vector<Actor*> m_actors;
        Socrates* m_socrates;
};

#endif // STUDENTWORLD_H_
