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
        void setAliveStatus(bool life);          // Nicer name than void kill()

        // All actors can collide, but in different ways
        virtual Actor* checkCollision(double x, double y);        // Check Collision at some X, Y
        virtual void onCollision(Actor* other);                   // Apply effects to Actor
        int distanceTo(Actor* other);       // Helper function

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
        void doSomething();
        Actor* checkCollision(double x, double y);        // Check Collision at some X, Y
        void onCollision(Actor* other);                   // Apply effects to Actor
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
        Actor* checkCollision(double x, double y);        // Check Collision at some X, Y
        void onCollision(Actor* other);                   // Apply effects to Actor
    private:
};

class Projectile : public Actor {
    public:
        Projectile(int imageID, double startX, double startY, StudentWorld* world, Direction dir, int lifeTime);
        ~Projectile();
        void doSomething();
        Actor* checkCollision(double x, double y);        // Check Collision at some X, Y
        void onCollision(Actor* other);                   // Apply effects to Actor
    private:
        int m_lifeTime;
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

#endif // ACTOR_H_
