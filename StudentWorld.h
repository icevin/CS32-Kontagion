#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "Actor.h"
#include "GameWorld.h"

#include <queue>
#include <string>
#include <vector>

class StudentWorld : public GameWorld {
  public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    int init();
    int move();
    void cleanUp();

    void addActor(Actor* a);
    void incEnemies() { m_nEnemies++; }
    void decEnemies() { m_nEnemies--; }

    bool hitCheck(double x, double y, double radius, Actor* orig, bool destructive = false, int hitFor = 0);
    bool findClosestFood(double& x, double& y, double radius, Actor* orig, bool destructive = false);

    double distToSoc(double fromX, double fromY);
    Direction dirToSoc(double fromX, double fromY);
    Direction directionTo(double fromX, double fromY, double toX, double toY);  // Helper function

    void hurtSoc(int amt);
    void healSoc();
    void addCharges(int amt);
  private:
    std::vector<Actor*> m_actors;
    std::queue<Actor*> checkOverlap(double x, double y, double radius, Actor* orig);
    template <class A>
    void populate(int num);
    template <class A>
    void addRadial();
    void spawnGoodies();
    double getDistance(Actor* a, Actor* b);

    Socrates* m_socrates;
    int m_nEnemies;
};

#endif  // STUDENTWORLD_H_
