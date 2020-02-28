#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class Actor : public GraphObject {
  public:
    Actor(
        int imageID,
        double startX,
        double startY,
        StudentWorld* world,
        Direction dir = 0,
        int depth     = 0,
        double size   = 1.0,
        bool alive    = true);
    virtual ~Actor();
    virtual void doSomething() = 0;

    // All actors can "die"
    virtual bool isAlive() { return m_living; }
    virtual void setAliveStatus(bool life);  // Nicer name than void kill()
    virtual void hurt(int amt);

    // Some actors are damageable by projectiles
    virtual bool isProjDamageable() { return false; }
    virtual bool hasHp() { return false; }
    virtual bool blocksMovement() { return false; }
    virtual bool isEdible() { return false; }

    // Actors need a StudentWorld pointer to interact with other actors
    StudentWorld* getStudentWorld() { return m_studentWorld; }

  private:
    bool m_living;
    StudentWorld* m_studentWorld;
};

class Dirt : public Actor {
  public:
    Dirt(double startX, double startY, StudentWorld* world);
    void doSomething();
    bool isProjDamageable() { return true; };
    bool blocksMovement() { return true; };
};

// Projectiles include Spray and Flame

class Projectile : public Actor {
  public:
    Projectile(int imageID, double startX, double startY, StudentWorld* world, Direction dir, int travelTime);
    void doSomething();
  protected:
    virtual bool checkHit() = 0;
  private:
    int m_travelTime;
};

class Flame : public Projectile {
  public:
    Flame(double startX, double startY, StudentWorld* world, Direction dir);
    bool checkHit();
};

class Spray : public Projectile {
  public:
    Spray(double startX, double startY, StudentWorld* world, Direction dir);
    bool checkHit();
};

// Food

class Food : public Actor {
  public:
    Food(double startX, double startY, StudentWorld* world);
    void doSomething() {}
    bool isEdible() { return true; };
};

// Pit

class Pit : public Actor {
  public:
    Pit(double startX, double startY, StudentWorld* world);
    ~Pit();
    void doSomething();
  private:
    enum bTypes { RegS,
                  AgrS,
                  ECol };
    int m_inventory[3];
};

// Goodies
// Goodies don't move, but can be hit by projectiles. They only affect Socrates.
// Subclasses: Restore health goodies, flamethrower goodies, extra life goodies, fungi

class Goodie : public Actor {
  public:
    Goodie(int imageID, double startX, double startY, StudentWorld* world);
    void doSomething();

    bool isAlive() { return m_lifeTime > 0; }
    void setAliveStatus(bool life) { m_lifeTime = life ? m_lifeTime : 0; };
    bool isProjDamageable() { return true; };
  protected:
    virtual void onPickup() = 0;
  private:
    int m_lifeTime;
};

class HealthGoodie : public Goodie {
  public:
    HealthGoodie(double startX, double startY, StudentWorld* world);
  protected:
    void onPickup();
};

class FlameThrowerGoodie : public Goodie {
  public:
    FlameThrowerGoodie(double startX, double startY, StudentWorld* world);
  protected:
    void onPickup();
};

class ExtraLifeGoodie : public Goodie {
  public:
    ExtraLifeGoodie(double startX, double startY, StudentWorld* world);
    void onPickup();
};

class Fungus : public Goodie {
  public:
    Fungus(double startX, double startY, StudentWorld* world);
    void onPickup();
};

// Divas are Actors that have hitpoints and are damageable (Socrates and Bacteria)

class Diva : public Actor {
  public:
    Diva(
        int imageID,
        double startX,
        double startY,
        StudentWorld* world,
        Direction dir,
        int health);
    virtual ~Diva();
    virtual void doSomething() = 0;

    virtual void onHurt()  = 0;
    virtual void onDeath() = 0;

    bool hasHp() { return true; };

    bool isAlive();
    void setAliveStatus(bool life) { m_health = life ? m_health : 0; };
    int getHealth() { return m_health; }
    void setHealth(int health);
    void hurt(int amt);

  private:
    int m_health;
};

class Socrates : public Diva {
  public:
    Socrates(StudentWorld* world);
    void doSomething();

    int getSCharges()         { return m_sprayCharges; }
    int getFCharges()         { return m_flameCharges; }
    void addFCharges(int amt) { m_flameCharges += amt; }

    void onHurt();
    void onDeath();

  private:
    void moveAlongCircle(int theta);
    int m_sprayCharges;
    int m_flameCharges;
};

// Bacteria

class Bacteria : public Diva {
  public:
    Bacteria(int imageID, double startX, double startY, StudentWorld* world, int health);
    ~Bacteria();
    bool isProjDamageable() { return true; };
    virtual void doSomething();
    virtual void onDeath();
  protected:
    bool tryMove(Direction dir, int units);
    virtual bool aggressiveBehavior();
    virtual void move()                       = 0;
    virtual void spawnNew(double x, double y) = 0;
    virtual void onOverlap()                  = 0;
    virtual void onHurt()                     = 0;
    virtual void uniqueOnDeath()              = 0;
  private:
    int m_nFoodEaten;
    int m_hitPoints;
};

// Salmonella are just regular Salmonella, but have AggressiveSalmonella as a subclass.
class Salmonella : public Bacteria {
  public:
    Salmonella(double startX, double startY, StudentWorld* world, int health = 4);
  protected:
    void move();
    void onHurt();
    void uniqueOnDeath();
    virtual bool aggressiveBehavior();
    virtual void onOverlap();
    virtual void spawnNew(double x, double y);

  private:
    int m_movementPlanDistance;
};

class AggressiveSalmonella : public Salmonella {
  public:
    AggressiveSalmonella(double startX, double startY, StudentWorld* world);
  protected:
    bool aggressiveBehavior();
    void onOverlap();
    void spawnNew(double x, double y);
};

class EColi : public Bacteria {
  public:
    EColi(double startX, double startY, StudentWorld* world);
  protected:
    void move();
    void onHurt();
    void uniqueOnDeath();
    void onOverlap();
    void spawnNew(double x, double y);
};
#endif  // ACTOR_H_