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
            int depth = 0, 
            double size = 1.0, 
            bool alive = true
        );
        virtual ~Actor();
        virtual void doSomething();

        // All actors can "die"
        virtual bool isAlive() {return m_living;}
        virtual void setAliveStatus(bool life);          // Nicer name than void kill()
        virtual void hurt(int amt);

        // Some actors are damageable by projectiles
        virtual bool isProjDamageable() = 0;
        virtual bool hasHp() {return false;};
        virtual bool blocksMovement() {return false;};
        virtual bool isEdible() {return false;};

        // Actors need a StudentWorld pointer to interact with other actors
        StudentWorld* getStudentWorld() {return m_studentWorld;}
    private:
        bool m_living;
        StudentWorld* m_studentWorld;
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
            int health
        );
        ~Diva();
        virtual void doSomething() = 0;

        bool isAlive();
        void setAliveStatus(bool life) {
            m_health = life ? m_health : 0;
        };
        int getHealth() {
            return m_health;
        }
        void setHealth(int health);
        void hurt(int amt);
        virtual void onHurt() = 0;
        virtual void onDeath() = 0;
        bool hasHp() {return true;};
    private:
        int m_health;
};

class Socrates : public Diva {
    public:
        Socrates(StudentWorld* world);
        ~Socrates();
        void doSomething();
        void onCollision(Actor* other);                   // Apply effects to Actor
        bool isProjDamageable() {return false;};

        int getSCharges() {
            return m_sprayCharges;
        }
        int getFCharges() {
            return m_flameCharges;
        }
        void addFCharges(int amt) {
            m_flameCharges += amt;
        }

        void onHurt();
        void onDeath();
    private:
        void moveAlongCircle(int theta);
        int m_sprayCharges;
        int m_flameCharges;
};


class Dirt : public Actor {
    public:
        Dirt(double startX, double startY, StudentWorld* world);
        ~Dirt();
        void doSomething();
        void onCollision(Actor* other);                   // Apply effects to Actor
        bool isProjDamageable() {
            return true;
        };
        bool blocksMovement() {
            return true;
        };
    private:
};


// Projectiles

class Projectile : public Actor {
    public:
        Projectile(int imageID, double startX, double startY, StudentWorld* world, Direction dir, int travelTime);
        ~Projectile();
        void doSomething();
        virtual bool checkHit() = 0;
        bool isProjDamageable(){return false;};
    private:
        int m_travelTime;
};

class Flame : public Projectile {
    public:
        Flame(double startX, double startY, StudentWorld* world, Direction dir);
        ~Flame();
        bool checkHit();
};

class Spray : public Projectile {
    public:
        Spray(double startX, double startY, StudentWorld* world, Direction dir);
        ~Spray();
        bool checkHit();
};


// PROPS
// Props don't move, and cannot be hit by projectiles. They may interact with bacteria. They are populated by StudentWorld::populate
// Subclasses: Food, Pits

class Prop : public Actor {
    public:
        Prop(int imageID, double startX, double startY, StudentWorld* world, Direction dir);
        ~Prop();
        virtual void doSomething();
        bool isProjDamageable() {return false;};
    private:
};

class Food : public Prop {
    public:
        Food(double startX, double startY, StudentWorld* world);
        ~Food();
        bool isEdible() {return true;};
};

class Pit : public Prop {
    public:
        Pit(double startX, double startY, StudentWorld* world);
        ~Pit();
        void doSomething();
    private:
        enum bTypes {RegS, AgrS, ECol};
        int m_inventory[3];
};


// Goodies
// Goodies don't move, but can be hit by projectiles. They only affect Socrates.
// Subclasses: Restore health goodies, flamethrower goodies, extra life goodies, fungi

class Goodie : public Actor {
    public:
        Goodie(int imageID, double startX, double startY, StudentWorld* world);
        ~Goodie();
        void doSomething();

        bool isAlive() {
            return m_lifeTime > 0;
        }
        void setAliveStatus(bool life) {
            m_lifeTime = life ? m_lifeTime : 0;
        };
        bool isProjDamageable() {
            return true;
        };

        virtual void onPickup() = 0;
    private:
        int m_lifeTime;
};

class HealthGoodie : public Goodie {
    public:
        HealthGoodie(double startX, double startY, StudentWorld* world);
        ~HealthGoodie();
        void onPickup();
};

class FlameThrowerGoodie : public Goodie {
    public:
        FlameThrowerGoodie(double startX, double startY, StudentWorld* world);
        ~FlameThrowerGoodie();
        void onPickup();
};

class ExtraLifeGoodie : public Goodie {
    public:
        ExtraLifeGoodie(double startX, double startY, StudentWorld* world);
        ~ExtraLifeGoodie();
        void onPickup();
};

class Fungus : public Goodie {
    public:
        Fungus(double startX, double startY, StudentWorld* world);
        ~Fungus();
        void onPickup();
};


// Bacteria

class Bacteria : public Diva {
    public:
        Bacteria(int imageID, double startX, double startY, StudentWorld* world, int health);
        ~Bacteria();
        bool tryMove(Direction dir, int units);
        bool isProjDamageable() {return true;};
        virtual void doSomething();
        virtual void move() = 0;
        virtual bool aggr() {
            return false;
        }
        virtual void spawnNew(double x, double y) = 0;
        virtual void onOverlap() = 0;
        virtual void onHurt() = 0;
        virtual void onDeath();
        virtual void uniqueOnDeath() = 0;
    private:
        int m_nFoodEaten;
        int m_hitPoints;
};

class Salmonella : public Bacteria {
    public:
        Salmonella(double startX, double startY, StudentWorld* world, int health = 4);
        ~Salmonella();
        void move();
        void onHurt();
        void uniqueOnDeath();
        virtual bool aggr();
        virtual void onOverlap();
        virtual void spawnNew(double x, double y);
    private:
        int m_movementPlanDistance;
};

class AggressiveSalmonella : public Salmonella {
    public:
        AggressiveSalmonella(double startX, double startY, StudentWorld* world);
        ~AggressiveSalmonella();
        bool aggr();
        void onOverlap();
        void spawnNew(double x, double y);
};

class EColi : public Bacteria {
    public:
        EColi(double startX, double startY, StudentWorld* world);
        ~EColi();
        void move();
        void onHurt();
        void uniqueOnDeath();
        void onOverlap();
        void spawnNew(double x, double y);
    private:
};
#endif // ACTOR_H_