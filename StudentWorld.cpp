#include "StudentWorld.h"

#include <string>
#include <vector>
#include <queue>

#ifndef _PI
#define _PI 3.14159265358979323846
#endif

using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), m_level(0)
{

}

StudentWorld::~StudentWorld() {
    cleanUp();
}

int StudentWorld::init()
{
    m_socrates = new Socrates(0, VIEW_HEIGHT/2, this);
    m_actors.push_back(m_socrates);
    int numDirt = ((180 - 20 * m_level) < 20) ? 20 : (180 - 20 * m_level);
    for (int i = 0; i < numDirt; i++)
    {
        int r = randInt(0, 120);
        int theta = randInt(0, 360);
        Dirt* temp = new Dirt(128 + r * cos(theta * _PI/180), 128 + r * sin(theta * _PI/180));
        m_actors.push_back(temp);
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    queue<Actor*> deadActors;
    
    for(Actor* a : m_actors) {
        if(a->isAlive())
            a->doSomething();
        else
            deadActors.push(a);
    }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for(Actor* a : m_actors) 
        if(a != nullptr)
            delete a;
    if(m_socrates != nullptr)
        delete m_socrates;
    return;
}
