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




/*              PROJECTILES             */




class Projectile : public Actor {
    public:
        Projectile(int imageID, double startX, double startY, StudentWorld* world, Direction dir, int lifeTime);
        ~Projectile();
        void doSomething();
        void onCollision(Actor* other);                   // Apply effects to Actor
        bool isProjDamageable(){return false;};
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




/*              PROPS               */




class Prop : public Actor {
    public:
        Prop(int imageID, double startX, double startY, StudentWorld* world, Direction dir);
        ~Prop();
        virtual void doSomething();
        
        // Some props die in differnt ways than others
        virtual bool isAlive();
        bool isProjDamageable() {return false;}
    private:
};
#endif // ACTOR_H_