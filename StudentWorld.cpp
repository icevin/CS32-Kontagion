#include "StudentWorld.h"

#include <string>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{

}

StudentWorld::~StudentWorld() {

}

int StudentWorld::init()
{
    m_socrates = new Socrates(0, VIEW_HEIGHT/2, this);
    m_actors.push_back(m_socrates);
    return GWSTATUS_CONTINUE_GAME;
}


//max(180 – 20 * L, 20)

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
    return;
}
