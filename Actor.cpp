#include "Actor.h"

#include <vector>

#include "GraphObject.h"
#include "StudentWorld.h"

#define _PI 3.14159265358979323846

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageID, double startX, double startY, Direction dir, int depth, double size)
: GraphObject(imageID, startX, startY, dir, depth, size)
{
    m_living = true;
}

Actor::~Actor() {
    return;
}

void Actor::doSomething() {

}

Socrates::Socrates(double startX, double startY, StudentWorld* world) 
: Actor(IID_PLAYER, startX, startY, 0, 0, 1.0), m_sprayCharges(0), m_flameCharges(0)
{
    m_studentWorld = world;
}

Socrates::~Socrates() {

}

void Socrates::doSomething() {
    // Get keypress:
    int keyPress = 0;
    if(m_studentWorld->getKey(keyPress)) {
        switch(keyPress) {
            case KEY_PRESS_LEFT:
            case 'a':
                moveAlongCircle(5);
                break;
            case KEY_PRESS_RIGHT:
            case 'd':
                moveAlongCircle(-5);
                break;
            case KEY_PRESS_SPACE:
                break;
            case KEY_PRESS_ENTER:
                break;
        }
    }
}

void Socrates::moveAlongCircle(int theta) {
    int currentAngle = 180 + getDirection();
    double newX = 128 + (VIEW_RADIUS) * cos((currentAngle + theta) * (_PI / 180));
    double newY = 128 + (VIEW_RADIUS) * sin((currentAngle + theta) * (_PI / 180));
    moveTo(newX, newY);
    setDirection(getDirection() + theta);
    return;
}

Dirt::Dirt(double startX, double startY) 
: Actor(IID_DIRT, startX, startY, 0, 1, 1)
{

}

Dirt::~Dirt() {

}