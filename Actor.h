#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class Actor : public GraphObject {
    public:
        Actor(int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0);
        virtual ~Actor();
        virtual void doSomething();
        virtual bool isAlive() {
            return m_living;
        }
    private:
        bool m_living;
};

class Socrates : public Actor {
    public:
        Socrates(double startX, double startY, StudentWorld* world);
        ~Socrates();
        void doSomething();
    private:
        void moveAlongCircle(int theta);
        StudentWorld* m_studentWorld;
        int m_sprayCharges;
        int m_flameCharges;
};

class Dirt : public Actor {
    public:
        Dirt(double startX, double startY);
        ~Dirt();
    private:
};

class Projectile : public Actor {
    public:
        Projectile(double startX, double startY, Direction dir);
        ~Projectile();
        void doSomething();
    private:
        int m_lifeTime;
};

class Flame : public Projectile {
    public:
        Flame(double startX, double startY, Direction dir);
};

class Spray : public Projectile {
    public:
        Spray(double startX, double startY, Direction dir);
        ~Spray();
        void doSomething();
};

#endif // ACTOR_H_
