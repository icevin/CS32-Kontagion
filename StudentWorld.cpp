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
    cerr << "StudentWorld constructed!" << endl;
    m_level = 1;
    m_nBaddies = 0;
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

int StudentWorld::init()
{
    // Create and add a new Socrates object at location (0, VIEW_HEIGHT/2)
    m_socrates = new Socrates(this);
    //m_actors.push_back(m_socrates);


    // Add min(5 * L, 25) food objects to the Petri dish
    populate<Food>(((5 * m_level) > 25) ? 25 : 5 * m_level);
    
    // Add max(180 –20 * L, 20) dirt at random locations
    populate<Dirt>((180 - (20 * m_level)) < 20 ? 20 : (180 - 20 * m_level));
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    queue<Actor*> dead_actors;
    m_socrates->doSomething();
    for(auto i = m_actors.begin(); i != m_actors.end(); i++) {
        Actor* a = (*i);
        if(a != nullptr) {
            if(a->isAlive()) {
                // cerr << "before " << a << " did something" << endl;
                a->doSomething();
                // cerr << "after " << a << " did something" << endl;
                if(!m_socrates->isAlive())
                    return GWSTATUS_PLAYER_DIED;
                if(!a->isAlive()) {
                    dead_actors.push(a);
                    (*i) = nullptr;
                    cerr << a << " died" << endl;
                }
            } else {
                dead_actors.push(a);
                (*i) = nullptr;
            }
        }
    }

    m_actors.erase(remove(begin(m_actors), end(m_actors), nullptr), end(m_actors));

    while(!dead_actors.empty()) {
        cerr << dead_actors.front() << endl;
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

bool StudentWorld::hitCheck(double x, double y, double radius, Actor* orig) {
    queue<Actor*> p_overlaps = checkOverlap(x, y, radius, orig);
    while(!p_overlaps.empty()) {
        if(p_overlaps.front() != nullptr && p_overlaps.front()->isProjDamageable()) {
            cerr << "hit" << p_overlaps.front()  << "|" << typeid(p_overlaps.front()).name() << endl;
            p_overlaps.front()->setAliveStatus(false);
            return true;
        } else {
            p_overlaps.pop();
        }
    }
}

queue<Actor*> StudentWorld::checkOverlap(double x, double y, double radius, Actor* orig) {
    queue<Actor*> overlaps;
    for(auto i = m_actors.begin(); i != m_actors.end(); i++) {
        Actor* a = (*i);
        if( a != nullptr && 
            sqrt(
                pow(
                    double(x - a->getX()),
                    2.0
                ) + pow(
                    double(y - a->getY()),
                    2.0
                )
            ) <= radius && 
            a != orig)
            overlaps.push(a);
    }
    return overlaps;
}

template<class A>
void StudentWorld::populate(int num) {
    for(int i = 0; i < num; i++) {
        int newX, newY;
        do {
            int r = randInt(0, 120);
            int theta = randInt(0, 360);
            newX = 128 + r * cos(theta * _PI/180);
            newY = 128 + r * sin(theta * _PI/180);
        } while (!checkOverlap(newX, newY, 2 * SPRITE_RADIUS, nullptr).empty());
        m_actors.push_back(new A(newX, newY, this));
    }
}