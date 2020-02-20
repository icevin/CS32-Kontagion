#include "Actor.h"

#include <vector>
#include <queue>
#include <typeinfo>
#include <iostream>

#include "GraphObject.h"
#include "StudentWorld.h"

#ifndef _PI
#define _PI 3.14159265358979323846
#endif

#ifndef _RAD_CONST
#define _RAD_CONST 1.0 / 360 * 2 * _PI
#endif

using namespace std;


//       db       .g8"""bgd MMP""MM""YMM   .g8""8q. `7MM"""Mq.  
//      ;MM:    .dP'     `M P'   MM   `7 .dP'    `YM. MM   `MM. 
//     ,V^MM.   dM'       `      MM      dM'      `MM MM   ,M9  
//    ,M  `MM   MM               MM      MM        MM MMmmdM9   
//    AbmmmqMA  MM.              MM      MM.      ,MP MM  YM.   
//   A'     VML `Mb.     ,'      MM      `Mb.    ,dP' MM   `Mb. 
// .AMA.   .AMMA. `"bmmmd'     .JMML.      `"bmmd"' .JMML. .JMM.

Actor::Actor(int imageID, double startX, double startY, StudentWorld* world, Direction dir, int depth, double size, bool isAlive)
: GraphObject(imageID, startX, startY, dir, depth, size), m_living(isAlive), m_studentWorld(world)
{

}

Actor::~Actor() {

}

void Actor::doSomething() {

}

void Actor::setAliveStatus(bool life) {
    m_living = life;
}

void Actor::onCollision(Actor* other) {

}

//  .M"""bgd   .g8""8q.     .g8"""bgd `7MM"""Mq.        db   MMP""MM""YMM `7MM"""YMM   .M"""bgd 
// ,MI    "Y .dP'    `YM. .dP'     `M   MM   `MM.      ;MM:  P'   MM   `7   MM    `7  ,MI    "Y 
// `MMb.     dM'      `MM dM'       `   MM   ,M9      ,V^MM.      MM        MM   d    `MMb.     
//   `YMMNq. MM        MM MM            MMmmdM9      ,M  `MM      MM        MMmmMM      `YMMNq. 
// .     `MM MM.      ,MP MM.           MM  YM.      AbmmmqMA     MM        MM   Y  , .     `MM 
// Mb     dM `Mb.    ,dP' `Mb.     ,'   MM   `Mb.   A'     VML    MM        MM     ,M Mb     dM 
// P"Ybmmd"    `"bmmd"'     `"bmmmd'  .JMML. .JMM..AMA.   .AMMA..JMML.    .JMMmmmmMMM P"Ybmmd"  

Socrates::Socrates(StudentWorld* world) 
: Actor(IID_PLAYER, 0, VIEW_HEIGHT/2, world, 0, 0, 1.0, true), m_sprayCharges(20), m_flameCharges(5), m_health(100)
{

}

Socrates::~Socrates() {

}

bool Socrates::isAlive() {
    return m_health > 0;
}

void Socrates::doSomething() {
    // Check if Socrates is alive:
    if(!isAlive())
        return;

    // Get keypress:
    int keyPress = 0;
    if(getStudentWorld()->getKey(keyPress)) {
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
                if(m_sprayCharges > 0) {
                    m_sprayCharges--;
                    getStudentWorld()->addActor(
                        new Spray(
                            getX() + SPRITE_WIDTH * cos(getDirection() * _RAD_CONST),
                            getY() + SPRITE_WIDTH * sin(getDirection() * _RAD_CONST),
                            getStudentWorld(),
                            getDirection()
                        )
                    );
                    getStudentWorld()->playSound(SOUND_PLAYER_SPRAY);
                }
                break;
            case KEY_PRESS_ENTER:
                if(m_flameCharges > 0) {
                    m_flameCharges--;
                    int tempDir = getDirection();
                    for (auto i = 0; i < 16; i++)
                    {
                        getStudentWorld()->addActor(
                            new Flame(
                                getX() + SPRITE_WIDTH * cos((tempDir + (22.0 * i)) * _RAD_CONST), 
                                getY() + SPRITE_WIDTH * sin((tempDir + (22.0 * i)) * _RAD_CONST), 
                                getStudentWorld(), 
                                tempDir + (22.0 * i)
                            )
                        );
                        getStudentWorld()->playSound(SOUND_PLAYER_FIRE);
                    }
                    
                }
                break;
        }
    } else if (m_sprayCharges < 20)
        m_sprayCharges++;
}

void Socrates::moveAlongCircle(int theta) {
    int currentAngle = 180 + getDirection();
    double newX = 128 + (VIEW_RADIUS) * cos((currentAngle + theta) * (_PI / 180));
    double newY = 128 + (VIEW_RADIUS) * sin((currentAngle + theta) * (_PI / 180));
    moveTo(newX, newY);
    setDirection(getDirection() + theta);
    return;
}

// `7MM"""Yb. `7MMF'`7MM"""Mq. MMP""MM""YMM 
//   MM    `Yb. MM    MM   `MM.P'   MM   `7 
//   MM     `Mb MM    MM   ,M9      MM      
//   MM      MM MM    MMmmdM9       MM      
//   MM     ,MP MM    MM  YM.       MM      
//   MM    ,dP' MM    MM   `Mb.     MM      
// .JMMmmmdP' .JMML..JMML. .JMM.  .JMML.    

Dirt::Dirt(double startX, double startY, StudentWorld* world) 
: Actor(IID_DIRT, startX, startY, world, 0, 1, 1, true)
{

}

Dirt::~Dirt() {

}

void Dirt::doSomething() {

}

void Dirt::onCollision(Actor* other) {

}
// `7MM"""Mq.`7MM"""Mq.   .g8""8q.     `7MMF'`7MM"""YMM    .g8"""bgd MMP""MM""YMM `7MMF'`7MMF'      `7MM"""YMM  
//   MM   `MM. MM   `MM..dP'    `YM.     MM    MM    `7  .dP'     `M P'   MM   `7   MM    MM          MM    `7  
//   MM   ,M9  MM   ,M9 dM'      `MM     MM    MM   d    dM'       `      MM        MM    MM          MM   d    
//   MMmmdM9   MMmmdM9  MM        MM     MM    MMmmMM    MM               MM        MM    MM          MMmmMM    
//   MM        MM  YM.  MM.      ,MP     MM    MM   Y  , MM.              MM        MM    MM      ,   MM   Y  , 
//   MM        MM   `Mb.`Mb.    ,dP'(O)  MM    MM     ,M `Mb.     ,'      MM        MM    MM     ,M   MM     ,M 
// .JMML.    .JMML. .JMM. `"bmmd"'   Ymmm9   .JMMmmmmMMM   `"bmmmd'     .JMML.    .JMML..JMMmmmmMMM .JMMmmmmMMM 

Projectile::Projectile(int imageID, double startX, double startY, StudentWorld* world, Direction dir, int travelTime) 
: Actor(imageID, startX, startY, world, dir, 1, 1, true), m_travelTime(travelTime)
{

}

Projectile::~Projectile() {

}

void Projectile::doSomething() {
    if(!isAlive())
        return;
    if(getStudentWorld()->hitCheck(getX(), getY(), SPRITE_RADIUS*2, this)) {
        setAliveStatus(false);
        return;
    }
    moveForward(SPRITE_RADIUS*2);
    m_travelTime -= SPRITE_RADIUS*2;
    if(m_travelTime < 0) {
        Actor::setAliveStatus(false);
        return;
    }
}

void Projectile::onCollision(Actor* other) {

}

// `7MM"""YMM `7MMF'            db      `7MMM.     ,MMF'`7MM"""YMM  
//   MM    `7   MM             ;MM:       MMMb    dPMM    MM    `7  
//   MM   d     MM            ,V^MM.      M YM   ,M MM    MM   d    
//   MM""MM     MM           ,M  `MM      M  Mb  M' MM    MMmmMM    
//   MM   Y     MM      ,    AbmmmqMA     M  YM.P'  MM    MM   Y  , 
//   MM         MM     ,M   A'     VML    M  `YM'   MM    MM     ,M 
// .JMML.     .JMMmmmmMMM .AMA.   .AMMA..JML. `'  .JMML..JMMmmmmMMM 

Flame::Flame(double startX, double startY, StudentWorld* world, Direction dir) 
: Projectile(IID_FLAME, startX, startY, world, dir, 32)
{

}

Flame::~Flame() {

}

//  .M"""bgd `7MM"""Mq.`7MM"""Mq.        db   `YMM'   `MM'
// ,MI    "Y   MM   `MM. MM   `MM.      ;MM:    VMA   ,V  
// `MMb.       MM   ,M9  MM   ,M9      ,V^MM.    VMA ,V   
//   `YMMNq.   MMmmdM9   MMmmdM9      ,M  `MM     VMMP    
// .     `MM   MM        MM  YM.      AbmmmqMA     MM     
// Mb     dM   MM        MM   `Mb.   A'     VML    MM     
// P"Ybmmd"  .JMML.    .JMML. .JMM..AMA.   .AMMA..JMML.     

Spray::Spray(double startX, double startY, StudentWorld* world, Direction dir)
: Projectile(IID_SPRAY, startX, startY, world, dir, 112) 
{

}

Spray::~Spray() {

}

// `7MM"""Mq.`7MM"""Mq.   .g8""8q. `7MM"""Mq.  .M"""bgd 
//   MM   `MM. MM   `MM..dP'    `YM. MM   `MM.,MI    "Y 
//   MM   ,M9  MM   ,M9 dM'      `MM MM   ,M9 `MMb.     
//   MMmmdM9   MMmmdM9  MM        MM MMmmdM9    `YMMNq. 
//   MM        MM  YM.  MM.      ,MP MM       .     `MM 
//   MM        MM   `Mb.`Mb.    ,dP' MM       Mb     dM 
// .JMML.    .JMML. .JMM. `"bmmd"' .JMML.     P"Ybmmd"  

Prop::Prop(int imageID, double startX, double startY, StudentWorld* world, Direction dir)
: Actor(imageID, startX, startY, world, dir, 1, 1, true)
{

}

Prop::~Prop() {

}

void Prop::doSomething() {

}

// `7MM"""YMM   .g8""8q.     .g8""8q. `7MM"""Yb.   
//   MM    `7 .dP'    `YM. .dP'    `YM. MM    `Yb. 
//   MM   d   dM'      `MM dM'      `MM MM     `Mb 
//   MM""MM   MM        MM MM        MM MM      MM 
//   MM   Y   MM.      ,MP MM.      ,MP MM     ,MP 
//   MM       `Mb.    ,dP' `Mb.    ,dP' MM    ,dP' 
// .JMML.       `"bmmd"'     `"bmmd"' .JMMmmmdP'   

Food::Food(double startX, double startY, StudentWorld* world) 
: Prop(IID_FOOD, startX, startY, world, 90)
{

}

Food::~Food() {

}
  
//   .g8"""bgd    .g8""8q.     .g8""8q. `7MM"""Yb. `7MMF'`7MM"""YMM  
// .dP'     `M  .dP'    `YM. .dP'    `YM. MM    `Yb. MM    MM    `7  
// dM'       `  dM'      `MM dM'      `MM MM     `Mb MM    MM   d    
// MM           MM        MM MM        MM MM      MM MM    MMmmMM    
// MM.    `7MMF'MM.      ,MP MM.      ,MP MM     ,MP MM    MM   Y  , 
// `Mb.     MM  `Mb.    ,dP' `Mb.    ,dP' MM    ,dP' MM    MM     ,M 
//   `"bmmmdPY    `"bmmd"'     `"bmmd"' .JMMmmmdP' .JMML..JMMmmmmMMM 

Goodie::Goodie(int imageID, double startX, double startY, StudentWorld* world, int lifeTime)
: Actor(imageID, startX, startY, world, 0, 1, 1, true), m_lifeTime(lifeTime)
{

}

Goodie::~Goodie() {

}

void Goodie::doSomething() {
    if(!isAlive())
        return;
}