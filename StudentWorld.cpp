#include "StudentWorld.h"

#include <algorithm>
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
: GameWorld(assetPath), m_level(0), m_nBaddies(0)
{
    cerr << "studentworld constructed!" << endl;
    m_level = 0;
    m_nBaddies = 0;
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

int StudentWorld::init()
{
    cerr << "init!" << endl;
    m_socrates = new Socrates(this);
    m_actors.push_back(m_socrates);
    int numDirt = ((180 - 20 * m_level) < 20) ? 20 : (180 - 20 * m_level);
    for (int i = 0; i < numDirt; i++)
    {
        int r = randInt(0, 120);
        int theta = randInt(0, 360);
        Dirt* temp = new Dirt(128 + r * cos(theta * _PI/180), 128 + r * sin(theta * _PI/180), this);
        m_actors.push_back(temp);
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    queue<Actor*> dead_actors;

    for(auto i = m_actors.begin(); i != m_actors.end(); i++) {
        Actor* a = (*i);
        if(a != nullptr) {
            if(a->isAlive()) {
               a->doSomething();
                if(!m_socrates->isAlive())
                    return GWSTATUS_PLAYER_DIED;
            } else {
                dead_actors.push(a);
                (*i) = nullptr;
            }
        }
    }

    m_actors.erase(remove(begin(m_actors), end(m_actors), nullptr), end(m_actors));

    while(!dead_actors.empty()) {
        if(dead_actors.front() != nullptr)
            delete dead_actors.front();
        dead_actors.pop();
    }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for(Actor* a : m_actors) 
        if(a != nullptr)
            delete a;
    m_actors.clear();
    if(m_socrates != nullptr)
        delete m_socrates;
    return;
}

void StudentWorld::addActor(Actor* a) {
    m_actors.push_back(a);
}