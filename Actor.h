#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;


class Actor : public GraphObject {
    public:
        Actor(int imageID, double startX, double startY, StudentWorld* world, Direction dir = 0, int depth = 0, double size = 1.0, bool alive = true);
        virtual ~Actor();
        virtual void doSomething();

        // All actors can "die"
        virtual bool isAlive() {return m_living;}
        virtual void setAliveStatus(bool life);          // Nicer name than void kill()

        // All actors can collide, but in different ways
        void onCollision(Actor* other);                   // Apply effects to Actor

        // Some actors are damageable by projectiles
        virtual bool isProjDamageable() = 0;

        // Actors need a StudentWorld pointer to interact with other actors
        StudentWorld* getStudentWorld() {return m_studentWorld;}
    private:
        bool m_living;
        StudentWorld* m_studentWorld;
};


class Socrates : public Actor {
    public:
        Socrates(StudentWorld* world);
        ~Socrates();
        bool isAlive();
        void setAliveStatus(bool life) {
            m_health = life ? m_health : 0;
        };
        void doSomething();
        void onCollision(Actor* other);                   // Apply effects to Actor
        bool isProjDamageable() {return false;};
    private:
        void moveAlongCircle(int theta);
        int m_sprayCharges;
        int m_flameCharges;
        int m_health;
};


class Dirt : public Actor {
    public:
        Dirt(double startX, double startY, StudentWorld* world);
        ~Dirt();
        void doSomething();
        void onCollision(Actor* other);                   // Apply effects to Actor
        bool isProjDamageable() {return true;};
    private:
};


// Projectiles

class Projectile : public Actor {
    public:
        Projectile(int imageID, double startX, double startY, StudentWorld* world, Direction dir, int travelTime);
        ~Projectile();
        void doSomething();
        void onCollision(Actor* other);                   // Apply effects to Actor
        bool isProjDamageable(){return false;};
    private:
        int m_travelTime;
};

class Flame : public Projectile {
    public:
        Flame(double startX, double startY, StudentWorld* world, Direction dir);
        ~Flame();
};

class Spray : public Projectile {
    public:
        Spray(double startX, double startY, StudentWorld* world, Direction dir);
        ~Spray();
};


// PROPS
// Props don't move, and cannot be hit by projectiles. They may interact with bacteria. They are populated by StudentWorld::populate
// Subclasses: Food, Pits

class Prop : public Actor {
    public:
        Prop(int imageID, double startX, double startY, StudentWorld* world, Direction dir);
        ~Prop();
        virtual void doSomething();
        bool isProjDamageable() {return false;}
    private:
};

class Food : public Prop {
    public:
        Food(double startX, double startY, StudentWorld* world);
        ~Food();
};

// class Pit :: public Prop {
//     public:
//         Pit()
// }


// Goodies
// Goodies don't move, but can be hit by projectiles. They only affect Socrates.
// Subclasses: Restore health goodies, flamethrower goodies, extra life goodies, fungi

class Goodie : public Actor {
    public:
        Goodie(int imageID, double startX, double startY, StudentWorld* world, int lifeTime);
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
#endif // ACTOR_H_