#include "StudentWorld.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

#ifndef _PI
#define _PI 3.14159265358979323846
#endif

using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), m_nBaddies(0)
{
    cerr << "StudentWorld constructed!" << endl;
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
    populate<Food>(min(5 * getLevel(), 25));
    
    // Add max(180 –20 * L, 20) dirt at random locations
    populate<Dirt>(max(180 - 20 * getLevel(), 20));
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // Give Each Actor a Chance to Do Something
    queue<Actor*> dead_actors;
    m_socrates->doSomething();
    for(auto i = m_actors.begin(); i != m_actors.end(); i++) {
        Actor* a = (*i);
        if(a != nullptr) {
            if(a->isAlive()) {
                a->doSomething();
                if(!m_socrates->isAlive())
                    return GWSTATUS_PLAYER_DIED;
                if(!a->isAlive()) {
                    dead_actors.push(a);
                    (*i) = nullptr;
                }
            } else {
                dead_actors.push(a);
                (*i) = nullptr;
            }
        }
    }

    // Remove Dead Actors
    m_actors.erase(remove(begin(m_actors), end(m_actors), nullptr), end(m_actors));
    while(!dead_actors.empty()) {
        if(dead_actors.front() != nullptr)
            delete dead_actors.front();
        dead_actors.pop();
    }

    int chanceFungus = max(510 - getLevel() * 10, 200);
    int genF = randInt(0, chanceFungus - 1);
    if(genF == 0) {      // Add fungus
        addRadial<Fungus>();
    }

    int chanceGoodie = max(510 - getLevel() * 10, 250);
    int genG = randInt(0, chanceGoodie - 1);
    if(genG == 0) {
        int gType = randInt(1, 10);
        if(gType == 1)
            addRadial<ExtraLifeGoodie>();
        else if(gType < 4)
            addRadial<FlameThrowerGoodie>();
        else
            addRadial<HealthGoodie>();
    }

    // Update Stat Text
    ostringstream titleText;
    titleText << setw(5) << "Score: " << setfill('0') << setw(6) << getScore() <<setfill(' ');
    titleText <<  "  Level: " << setw(2) << getLevel();
    titleText <<  "  Lives: " <<  setw(1) << getLives();
    titleText <<  "  Health: " <<  setw(3) << m_socrates->getHealth();
    titleText <<  "  Sprays: " <<  setw(2) << m_socrates->getSCharges();
    titleText <<  "  Flames: " <<  setw(2) << m_socrates->getFCharges();

    setGameStatText(titleText.str());    

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

bool StudentWorld::socCheck(double x, double y, double radius) {
    return radius >= sqrt(
                pow(double(x - m_socrates->getX()), 2.0) + 
                pow(double(y - m_socrates->getY()), 2.0)
            );
}

void StudentWorld::hurtSoc(int amt) {
    m_socrates->setHealth(m_socrates->getHealth() - amt);
}

void StudentWorld::healSoc() {
    m_socrates->setHealth(100);
}

void StudentWorld::addCharges(int amt) {
    m_socrates->addFCharges(amt);
}

queue<Actor*> StudentWorld::checkOverlap(double x, double y, double radius, Actor* orig) {
    queue<Actor*> overlaps;
    for(auto i = m_actors.begin(); i != m_actors.end(); i++) {
        Actor* a = (*i);
        if( 
            a != nullptr && 
            sqrt(
                pow(double(x - a->getX()), 2.0) + 
                pow(double(y - a->getY()), 2.0)
            ) <= radius && 
            a != orig
        )
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
            newX = (VIEW_WIDTH/2) + r * cos(theta * _PI/180);
            newY = (VIEW_HEIGHT/2) + r * sin(theta * _PI/180);
        } while (!checkOverlap(newX, newY, 2 * SPRITE_RADIUS, nullptr).empty());
        addActor(new A(newX, newY, this));
    }
}

template<class A>
void StudentWorld::addRadial() {
    int theta = randInt(0, 360);
    addActor(new A(
        (VIEW_WIDTH/2) + VIEW_RADIUS * cos(theta * _PI/180), 
        (VIEW_HEIGHT/2) + VIEW_RADIUS * sin(theta * _PI/180), 
        this
    ));
}