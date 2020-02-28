#include "StudentWorld.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

#ifndef _PI
#define _PI 3.14159265358979323846
#endif

using namespace std;

GameWorld* createStudentWorld(string assetPath) {
    return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath), m_nEnemies(0) {

}

StudentWorld::~StudentWorld() {
    cleanUp();
}

int StudentWorld::init() {
    // Create and add a new Socrates object at location (0, VIEW_HEIGHT/2)
    m_socrates = new Socrates(this);

    //m_actors.push_back(m_socrates);

    // Reset number of enemies
    m_nEnemies = 0;

    // Add L pits to the Petri dish
    populate<Pit>(getLevel());

    // Add min(5 * L, 25) food objects to the Petri dish
    populate<Food>(min(5 * getLevel(), 25));

    // Add max(180 –20 * L, 20) dirt to the Petri dish
    populate<Dirt>(max(180 - 20 * getLevel(), 20));

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
    // Give Each Actor a Chance to Do Something
    queue<Actor*> dead_actors;
    m_socrates->doSomething();
    for (auto i = m_actors.begin(); i != m_actors.end(); i++) {
        Actor* a = (*i);
        if (a != nullptr) {
            if (a->isAlive()) {
                a->doSomething();
                if (!m_socrates->isAlive()) {
                    decLives();
                    return GWSTATUS_PLAYER_DIED;
                }
                if (!a->isAlive()) {
                    dead_actors.push(a);
                    (*i) = nullptr;
                }
                if (m_nEnemies <= 0) {
                    playSound(SOUND_FINISHED_LEVEL);
                    return GWSTATUS_FINISHED_LEVEL;
                }
            } else {
                dead_actors.push(a);
                (*i) = nullptr;
            }
        }
    }

    // Remove Dead Actors
    m_actors.erase(remove(begin(m_actors), end(m_actors), nullptr), end(m_actors));
    while (!dead_actors.empty()) {
        if (dead_actors.front() != nullptr)
            delete dead_actors.front();
        dead_actors.pop();
    }

    // Add potential new actors
    spawnGoodies();

    // Update Stat Text
    ostringstream statText;
    statText << setw(5) << "Score: ";
    if (getScore() >= 0)                                        // Keep the Score display 6 digits regardless if the score is negative or not
        statText << setfill('0') << setw(6) << getScore();
    else
        statText << "-" << setfill('0') << setw(5) << abs(getScore());
    statText << setfill(' ');
    statText << "  Level: " << setw(2) << getLevel();
    statText << "  Lives: " << setw(1) << getLives();
    statText << "  Health: " << setw(3) << m_socrates->getHealth();
    statText << "  Sprays: " << setw(2) << m_socrates->getSCharges();
    statText << "  Flames: " << setw(2) << m_socrates->getFCharges();

    setGameStatText(statText.str());

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
    for (Actor* a : m_actors)       // Delete all actors in m_actors
        if (a != nullptr)
            delete a;
    m_actors.clear();               // Clear the vector so that it can be used in the next iteration
    if (m_socrates != nullptr)
        delete m_socrates;
    return;
}

void StudentWorld::addActor(Actor* a) {
    m_actors.push_back(a);
}

bool StudentWorld::hitCheck(double x, double y, double radius, Actor* orig, bool destructive, int hitFor) {
    queue<Actor*> p_overlaps = checkOverlap(x, y, radius, orig);        // There can potentially be multiple overlaps, so we store them in a queue
    while (!p_overlaps.empty()) {
        if (p_overlaps.front() != nullptr && ((destructive && p_overlaps.front()->isProjDamageable()) || p_overlaps.front()->blocksMovement())) { // We only accept actors that are destructible if we are destructively checking for a hit, or actors that block movement if we are non-destructively looking for a hit     
            if (destructive) {          // Either hurt or destroy the actor, depending on if it is destructible or not
                if (p_overlaps.front()->hasHp()) {                  
                    p_overlaps.front()->hurt(hitFor);
                } else {
                    p_overlaps.front()->setAliveStatus(false);
                }
            }
            return true;            // Notify the calling function that we have found a valid overlap
        } else {
            p_overlaps.pop();
        }
    }
    return false;
}

bool StudentWorld::findClosestFood(double& x, double& y, double radius, Actor* orig, bool destructive) {
    map<double, Actor*> menu;       // Use a map because it automatically sorts by the first variable in the pair
    queue<Actor*> p_overlaps = checkOverlap(x, y, radius, orig);    // Get queue of overlaps
    while (!p_overlaps.empty()) {
        if (p_overlaps.front() != nullptr && p_overlaps.front()->isEdible())        
            menu.insert(pair<double, Actor*>(getDistance(orig, p_overlaps.front()), p_overlaps.front()));   // Add all valid (edible) objects into the menu
        p_overlaps.pop();
    }
    if (!menu.empty()) {
        x = menu.begin()->second->getX();   // Get the first item from the menu (it will have the lowest distance)
        y = menu.begin()->second->getY();
        if (destructive) {
            menu.begin()->second->setAliveStatus(false);    // If we are destructively looking for food, destroy the closest food
            return true;
        }
    }
    return !menu.empty();       // Return true if the menu is not empty, false if we didn't find any valid food
}

double StudentWorld::distToSoc(double fromX, double fromY) {
    return sqrt(
        pow(double(fromX - m_socrates->getX()), 2.0) +
        pow(double(fromY - m_socrates->getY()), 2.0)
    );
}

Direction StudentWorld::dirToSoc(double fromX, double fromY) {
    return directionTo(fromX, fromY, m_socrates->getX(), m_socrates->getY());
}

Direction StudentWorld::directionTo(double fromX, double fromY, double toX, double toY) {
    if (fromX == toX && fromY == toY)
        return -999;  // Domain error
    return atan2(toY - fromY, toX - fromX) * 180.0 / _PI;
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
    queue<Actor*> overlaps;     // There can potentially be multiple overlaps, so we use a queue to store all of them
    for (auto i = m_actors.begin(); i != m_actors.end(); i++) {
        Actor* a = (*i);
        if (
            a != nullptr &&     // If a isn't a nullptr
            sqrt(pow(double(x - a->getX()), 2.0) + pow(double(y - a->getY()), 2.0)) <= radius &&    // And it is within range
            a != orig)      // And it is not the same actor as the actor that is calling this function,
            overlaps.push(a);   // Add the actor to the queue
    }
    return overlaps; // Return the queue of actors
}

// Helper function to add objects to the petri dish randomly (used for Pits, Food, and Dirt)
template <class A>
void StudentWorld::populate(int num) {
    for (int i = 0; i < num; i++) {
        int newX, newY;
        do {
            int r     = randInt(0, 120);        // Generate random polar coordinates and convert to Cartesian
            int theta = randInt(0, 360);
            newX      = (VIEW_WIDTH / 2) + r * cos(theta * _PI / 180);
            newY      = (VIEW_HEIGHT / 2) + r * sin(theta * _PI / 180);
        } while (!checkOverlap(newX, newY, SPRITE_WIDTH, nullptr).empty()); // Keep generating new coordinates if there is overlap
        addActor(new A(newX, newY, this));
    }
}

// Helper function to add objects along the circumference of the petri dish (used to spawn Goodies)
template <class A>
void StudentWorld::addRadial() {
    int theta = randInt(0, 360);
    addActor(new A(
        (VIEW_WIDTH / 2) + VIEW_RADIUS * cos(theta * _PI / 180),
        (VIEW_HEIGHT / 2) + VIEW_RADIUS * sin(theta * _PI / 180),
        this));
}

// Helper function to spawn the Goodies randomly
void StudentWorld::spawnGoodies() {
    int chanceFungus = max(510 - getLevel() * 10, 200);
    int genF         = randInt(0, chanceFungus - 1);
    if (genF == 0) {  // Add fungus
        addRadial<Fungus>();
    }

    int chanceGoodie = max(510 - getLevel() * 10, 250);
    int genG         = randInt(0, chanceGoodie - 1);
    if (genG == 0) {
        int gType = randInt(1, 10);
        if (gType == 1)
            addRadial<ExtraLifeGoodie>();
        else if (gType < 4)
            addRadial<FlameThrowerGoodie>();
        else
            addRadial<HealthGoodie>();
    }
}

// Helper function to get the distance between two actors
double StudentWorld::getDistance(Actor* a, Actor* b) {
    return sqrt(
        pow(double(b->getX() - a->getX()), 2.0) +
        pow(double(b->getY() - a->getY()), 2.0));
}