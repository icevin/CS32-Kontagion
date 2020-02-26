#include "Actor.h"

#include <algorithm>
#include <iostream>
#include <queue>
#include <typeinfo>
#include <vector>

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
    : GraphObject(imageID, startX, startY, dir, depth, size), m_living(isAlive), m_studentWorld(world) {
}

Actor::~Actor() {
}

void Actor::doSomething() {
}

void Actor::setAliveStatus(bool life) {
    m_living = life;
}

void Actor::hurt(int amt) {
    m_living = amt > 0 ? true : false;
}

// DIVAS

Diva::Diva(int imageID, double startX, double startY, StudentWorld* world, Direction dir, int health)
    : Actor(imageID, startX, startY, world, dir, 0, 1, true), m_health(health) {
}

Diva::~Diva() {
}

bool Diva::isAlive() {
    return m_health > 0;
}

void Diva::setHealth(int health) {
    if (health < m_health && health > 0)
        onHurt();
    m_health = health;
    if (m_health <= 0)
        onDeath();
}

void Diva::hurt(int amt) {
    setHealth(m_health - amt);
}

//  .M"""bgd   .g8""8q.     .g8"""bgd `7MM"""Mq.        db   MMP""MM""YMM `7MM"""YMM   .M"""bgd
// ,MI    "Y .dP'    `YM. .dP'     `M   MM   `MM.      ;MM:  P'   MM   `7   MM    `7  ,MI    "Y
// `MMb.     dM'      `MM dM'       `   MM   ,M9      ,V^MM.      MM        MM   d    `MMb.
//   `YMMNq. MM        MM MM            MMmmdM9      ,M  `MM      MM        MMmmMM      `YMMNq.
// .     `MM MM.      ,MP MM.           MM  YM.      AbmmmqMA     MM        MM   Y  , .     `MM
// Mb     dM `Mb.    ,dP' `Mb.     ,'   MM   `Mb.   A'     VML    MM        MM     ,M Mb     dM
// P"Ybmmd"    `"bmmd"'     `"bmmmd'  .JMML. .JMM..AMA.   .AMMA..JMML.    .JMMmmmmMMM P"Ybmmd"

Socrates::Socrates(StudentWorld* world)
    : Diva(IID_PLAYER, 0, VIEW_HEIGHT / 2, world, 0, 100), m_sprayCharges(20), m_flameCharges(5) {
}

Socrates::~Socrates() {
}

void Socrates::doSomething() {
    // Check if Socrates is alive:
    if (!isAlive())
        return;

    // Get keypress:
    int keyPress = 0;
    if (getStudentWorld()->getKey(keyPress)) {
        switch (keyPress) {
            case KEY_PRESS_LEFT:
            case 'a':
                moveAlongCircle(5);
                break;
            case KEY_PRESS_RIGHT:
            case 'd':
                moveAlongCircle(-5);
                break;
            case KEY_PRESS_SPACE:
                if (m_sprayCharges > 0) {
                    m_sprayCharges--;
                    getStudentWorld()->addActor(
                        new Spray(
                            getX() + SPRITE_WIDTH * cos(getDirection() * _RAD_CONST),
                            getY() + SPRITE_WIDTH * sin(getDirection() * _RAD_CONST),
                            getStudentWorld(),
                            getDirection()));
                    getStudentWorld()->playSound(SOUND_PLAYER_SPRAY);
                }
                break;
            case KEY_PRESS_ENTER:
                if (m_flameCharges > 0) {
                    m_flameCharges--;
                    int tempDir = getDirection();
                    for (auto i = 0; i < 16; i++) {
                        getStudentWorld()->addActor(
                            new Flame(
                                getX() + SPRITE_WIDTH * cos((tempDir + (22.0 * i)) * _RAD_CONST),
                                getY() + SPRITE_WIDTH * sin((tempDir + (22.0 * i)) * _RAD_CONST),
                                getStudentWorld(),
                                tempDir + (22.0 * i)));
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
    double newX      = 128 + (VIEW_RADIUS)*cos((currentAngle + theta) * (_PI / 180));
    double newY      = 128 + (VIEW_RADIUS)*sin((currentAngle + theta) * (_PI / 180));
    moveTo(newX, newY);
    setDirection(getDirection() + theta);
    return;
}

void Socrates::onHurt() {
    cerr << "PLAY SOUND_PLAYER_HURT" << endl;
    getStudentWorld()->playSound(SOUND_PLAYER_HURT);
}

void Socrates::onDeath() {
    cerr << "PLAY SOUND_PLAYER_DIE" << endl;
    getStudentWorld()->playSound(SOUND_PLAYER_DIE);
}

// `7MM"""Yb. `7MMF'`7MM"""Mq. MMP""MM""YMM
//   MM    `Yb. MM    MM   `MM.P'   MM   `7
//   MM     `Mb MM    MM   ,M9      MM
//   MM      MM MM    MMmmdM9       MM
//   MM     ,MP MM    MM  YM.       MM
//   MM    ,dP' MM    MM   `Mb.     MM
// .JMMmmmdP' .JMML..JMML. .JMM.  .JMML.

Dirt::Dirt(double startX, double startY, StudentWorld* world)
    : Actor(IID_DIRT, startX, startY, world, 0, 1, 1, true) {
}

Dirt::~Dirt() {
}

void Dirt::doSomething() {
    int aaaa = 12;
    int b    = 23;
    int ccc  = 23;
}

// `7MM"""Mq.`7MM"""Mq.   .g8""8q.     `7MMF'`7MM"""YMM    .g8"""bgd MMP""MM""YMM `7MMF'`7MMF'      `7MM"""YMM
//   MM   `MM. MM   `MM..dP'    `YM.     MM    MM    `7  .dP'     `M P'   MM   `7   MM    MM          MM    `7
//   MM   ,M9  MM   ,M9 dM'      `MM     MM    MM   d    dM'       `      MM        MM    MM          MM   d
//   MMmmdM9   MMmmdM9  MM        MM     MM    MMmmMM    MM               MM        MM    MM          MMmmMM
//   MM        MM  YM.  MM.      ,MP     MM    MM   Y  , MM.              MM        MM    MM      ,   MM   Y  ,
//   MM        MM   `Mb.`Mb.    ,dP'(O)  MM    MM     ,M `Mb.     ,'      MM        MM    MM     ,M   MM     ,M
// .JMML.    .JMML. .JMM. `"bmmd"'   Ymmm9   .JMMmmmmMMM   `"bmmmd'     .JMML.    .JMML..JMMmmmmMMM .JMMmmmmMMM

Projectile::Projectile(int imageID, double startX, double startY, StudentWorld* world, Direction dir, int travelTime)
    : Actor(imageID, startX, startY, world, dir, 1, 1, true), m_travelTime(travelTime) {
}

Projectile::~Projectile() {
}

void Projectile::doSomething() {
    if (!isAlive())
        return;
    if (checkHit()) {
        setAliveStatus(false);
        return;
    }
    moveForward(SPRITE_RADIUS * 2);
    m_travelTime -= SPRITE_RADIUS * 2;
    if (m_travelTime < 0) {
        Actor::setAliveStatus(false);
        return;
    }
}

// `7MM"""YMM `7MMF'            db      `7MMM.     ,MMF'`7MM"""YMM
//   MM    `7   MM             ;MM:       MMMb    dPMM    MM    `7
//   MM   d     MM            ,V^MM.      M YM   ,M MM    MM   d
//   MM""MM     MM           ,M  `MM      M  Mb  M' MM    MMmmMM
//   MM   Y     MM      ,    AbmmmqMA     M  YM.P'  MM    MM   Y  ,
//   MM         MM     ,M   A'     VML    M  `YM'   MM    MM     ,M
// .JMML.     .JMMmmmmMMM .AMA.   .AMMA..JML. `'  .JMML..JMMmmmmMMM

Flame::Flame(double startX, double startY, StudentWorld* world, Direction dir)
    : Projectile(IID_FLAME, startX, startY, world, dir, 32) {
}

Flame::~Flame() {
}

bool Flame::checkHit() {
    return getStudentWorld()->hitCheck(getX(), getY(), SPRITE_RADIUS * 2, this, true, 5);
}

//  .M"""bgd `7MM"""Mq.`7MM"""Mq.        db   `YMM'   `MM'
// ,MI    "Y   MM   `MM. MM   `MM.      ;MM:    VMA   ,V
// `MMb.       MM   ,M9  MM   ,M9      ,V^MM.    VMA ,V
//   `YMMNq.   MMmmdM9   MMmmdM9      ,M  `MM     VMMP
// .     `MM   MM        MM  YM.      AbmmmqMA     MM
// Mb     dM   MM        MM   `Mb.   A'     VML    MM
// P"Ybmmd"  .JMML.    .JMML. .JMM..AMA.   .AMMA..JMML.

Spray::Spray(double startX, double startY, StudentWorld* world, Direction dir)
    : Projectile(IID_SPRAY, startX, startY, world, dir, 112) {
}

Spray::~Spray() {
}

bool Spray::checkHit() {
    return getStudentWorld()->hitCheck(getX(), getY(), SPRITE_RADIUS * 2, this, true, 2);
}

// `7MM"""Mq.`7MM"""Mq.   .g8""8q. `7MM"""Mq.  .M"""bgd
//   MM   `MM. MM   `MM..dP'    `YM. MM   `MM.,MI    "Y
//   MM   ,M9  MM   ,M9 dM'      `MM MM   ,M9 `MMb.
//   MMmmdM9   MMmmdM9  MM        MM MMmmdM9    `YMMNq.
//   MM        MM  YM.  MM.      ,MP MM       .     `MM
//   MM        MM   `Mb.`Mb.    ,dP' MM       Mb     dM
// .JMML.    .JMML. .JMM. `"bmmd"' .JMML.     P"Ybmmd"

Prop::Prop(int imageID, double startX, double startY, StudentWorld* world, Direction dir)
    : Actor(imageID, startX, startY, world, dir, 1, 1, true) {
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
    : Prop(IID_FOOD, startX, startY, world, 90) {
}

Food::~Food() {
}

// `7MM"""Mq.`7MMF'MMP""MM""YMM
//   MM   `MM. MM  P'   MM   `7
//   MM   ,M9  MM       MM
//   MMmmdM9   MM       MM
//   MM        MM       MM
//   MM        MM       MM
// .JMML.    .JMML.   .JMML.

Pit::Pit(double startX, double startY, StudentWorld* world)
    : Prop(IID_PIT, startX, startY, world, 0) {
    m_inventory[RegS] = 5;  // 5
    m_inventory[AgrS] = 3;  // 3
    m_inventory[ECol] = 2;  // 2
    getStudentWorld()->incEnemies();
}

Pit::~Pit() {
    getStudentWorld()->decEnemies();
}

void Pit::doSomething() {
    if (m_inventory[RegS] + m_inventory[AgrS] + m_inventory[ECol] == 0) {
        setAliveStatus(false);
    }
    if (randInt(1, 50) == 1) {
        int type = RegS;
        do {
            type = randInt(RegS, ECol);
        } while (m_inventory[type] <= 0 && m_inventory[RegS] + m_inventory[AgrS] + m_inventory[ECol] != 0);
        if (type == RegS)
            getStudentWorld()->addActor(new Salmonella(getX(), getY(), getStudentWorld()));
        if (type == AgrS)
            getStudentWorld()->addActor(new AggressiveSalmonella(getX(), getY(), getStudentWorld()));
        if (type == ECol)
            getStudentWorld()->addActor(new EColi(getX(), getY(), getStudentWorld()));
        m_inventory[type]--;
        getStudentWorld()->playSound(SOUND_BACTERIUM_BORN);
    }
}

//   .g8"""bgd    .g8""8q.     .g8""8q. `7MM"""Yb. `7MMF'`7MM"""YMM
// .dP'     `M  .dP'    `YM. .dP'    `YM. MM    `Yb. MM    MM    `7
// dM'       `  dM'      `MM dM'      `MM MM     `Mb MM    MM   d
// MM           MM        MM MM        MM MM      MM MM    MMmmMM
// MM.    `7MMF'MM.      ,MP MM.      ,MP MM     ,MP MM    MM   Y  ,
// `Mb.     MM  `Mb.    ,dP' `Mb.    ,dP' MM    ,dP' MM    MM     ,M
//   `"bmmmdPY    `"bmmd"'     `"bmmd"' .JMMmmmdP' .JMML..JMMmmmmMMM

Goodie::Goodie(int imageID, double startX, double startY, StudentWorld* world)
    : Actor(imageID, startX, startY, world, 0, 1, 1, true), m_lifeTime(max(randInt(0, 300 - 10 * world->getLevel() - 1), 50)) {
}

Goodie::~Goodie() {
}

void Goodie::doSomething() {
    if (!isAlive())
        return;
    if (getStudentWorld()->distToSoc(getX(), getY()) <= SPRITE_WIDTH) {
        onPickup();
        setAliveStatus(false);
        return;
    }
    if (--m_lifeTime <= 0)
        setAliveStatus(false);
}

// Health Goodie

HealthGoodie::HealthGoodie(double startX, double startY, StudentWorld* world)
    : Goodie(IID_RESTORE_HEALTH_GOODIE, startX, startY, world) {
}

HealthGoodie::~HealthGoodie() {
}

void HealthGoodie::onPickup() {
    getStudentWorld()->increaseScore(250);
    getStudentWorld()->playSound(SOUND_GOT_GOODIE);
    getStudentWorld()->healSoc();
    setAliveStatus(false);
}

// Flame Thrower Goodie

FlameThrowerGoodie::FlameThrowerGoodie(double startX, double startY, StudentWorld* world)
    : Goodie(IID_FLAME_THROWER_GOODIE, startX, startY, world) {
}

FlameThrowerGoodie::~FlameThrowerGoodie() {
}

void FlameThrowerGoodie::onPickup() {
    getStudentWorld()->increaseScore(300);
    getStudentWorld()->playSound(SOUND_GOT_GOODIE);
    getStudentWorld()->addCharges(5);
    setAliveStatus(false);
}

//Extra Life Goodie

ExtraLifeGoodie::ExtraLifeGoodie(double startX, double startY, StudentWorld* world)
    : Goodie(IID_EXTRA_LIFE_GOODIE, startX, startY, world) {
}

ExtraLifeGoodie::~ExtraLifeGoodie() {
}

void ExtraLifeGoodie::onPickup() {
    getStudentWorld()->increaseScore(500);
    getStudentWorld()->playSound(SOUND_GOT_GOODIE);
    getStudentWorld()->incLives();
    setAliveStatus(false);
}

//

Fungus::Fungus(double startX, double startY, StudentWorld* world)
    : Goodie(IID_FUNGUS, startX, startY, world) {
}

Fungus::~Fungus() {
}

void Fungus::onPickup() {
    getStudentWorld()->increaseScore(-50);
    getStudentWorld()->hurtSoc(20);
    setAliveStatus(false);
}

// `7MM"""Yp,      db       .g8"""bgd MMP""MM""YMM `7MM"""YMM  `7MM"""Mq.  `7MMF'      db
//   MM    Yb     ;MM:    .dP'     `M P'   MM   `7   MM    `7    MM   `MM.   MM       ;MM:
//   MM    dP    ,V^MM.   dM'       `      MM        MM   d      MM   ,M9    MM      ,V^MM.
//   MM"""bg.   ,M  `MM   MM               MM        MMmmMM      MMmmdM9     MM     ,M  `MM
//   MM    `Y   AbmmmqMA  MM.              MM        MM   Y  ,   MM  YM.     MM     AbmmmqMA
//   MM    ,9  A'     VML `Mb.     ,'      MM        MM     ,M   MM   `Mb.   MM    A'     VML
// .JMMmmmd9 .AMA.   .AMMA. `"bmmmd'     .JMML.    .JMMmmmmMMM .JMML. .JMM..JMML..AMA.   .AMMA.

Bacteria::Bacteria(int imageID, double startX, double startY, StudentWorld* world, int health)
    : Diva(imageID, startX, startY, world, 90, health), m_nFoodEaten(0) {
    getStudentWorld()->incEnemies();
}

Bacteria::~Bacteria() {
    getStudentWorld()->decEnemies();
}

bool Bacteria::tryMove(Direction dir, int units) {
    setDirection(dir);
    double newX = 0, newY = 0;
    getPositionInThisDirection(dir, units, newX, newY);
    if (!getStudentWorld()->hitCheck(newX, newY, SPRITE_WIDTH / 2, this)) {
        if (sqrt(pow(double(newX - VIEW_WIDTH / 2.0), 2.0) + pow(double(newY - VIEW_HEIGHT / 2.0), 2.0)) < VIEW_RADIUS) {
            moveAngle(dir, units);
            return true;
        }
    }
    return false;
}

void Bacteria::doSomething() {
    if (!isAlive())
        return;
    bool mode = aggr();
    if (getStudentWorld()->distToSoc(getX(), getY()) <= SPRITE_WIDTH)
        onOverlap();
    else {
        double newX = getX();
        double newY = getY();
        if (m_nFoodEaten >= 3) {
            newX += ((getX() < VIEW_WIDTH / 2) ? 1 : -1) * SPRITE_RADIUS;
            newY += ((getY() < VIEW_HEIGHT / 2) ? 1 : -1) * SPRITE_RADIUS;
            spawnNew(newX, newY);
            m_nFoodEaten = 0;
        } else if (getStudentWorld()->findClosestFood(newX, newY, SPRITE_WIDTH, this, true)) {
            m_nFoodEaten++;
        }
    }
    if (!mode)
        move();
}

void Bacteria::onDeath() {
    getStudentWorld()->increaseScore(100);
    if (randInt(1, 2) == 1)
        getStudentWorld()->addActor(new Food(getX(), getY(), getStudentWorld()));
}

//  .M"""bgd      db      `7MMF'      `7MMM.     ,MMF' .g8""8q. `7MN.   `7MF'`7MM"""YMM  `7MMF'      `7MMF'            db
// ,MI    "Y     ;MM:       MM          MMMb    dPMM .dP'    `YM. MMN.    M    MM    `7    MM          MM             ;MM:
// `MMb.        ,V^MM.      MM          M YM   ,M MM dM'      `MM M YMb   M    MM   d      MM          MM            ,V^MM.
//   `YMMNq.   ,M  `MM      MM          M  Mb  M' MM MM        MM M  `MN. M    MMmmMM      MM          MM           ,M  `MM
// .     `MM   AbmmmqMA     MM      ,   M  YM.P'  MM MM.      ,MP M   `MM.M    MM   Y  ,   MM      ,   MM      ,    AbmmmqMA
// Mb     dM  A'     VML    MM     ,M   M  `YM'   MM `Mb.    ,dP' M     YMM    MM     ,M   MM     ,M   MM     ,M   A'     VML
// P"Ybmmd" .AMA.   .AMMA..JMMmmmmMMM .JML. `'  .JMML. `"bmmd"' .JML.    YM  .JMMmmmmMMM .JMMmmmmMMM .JMMmmmmMMM .AMA.   .AMMA.

Salmonella::Salmonella(double startX, double startY, StudentWorld* world, int health)
    : Bacteria(IID_SALMONELLA, startX, startY, world, health), m_movementPlanDistance(0) {
}

Salmonella::~Salmonella() {
}

void Salmonella::onHurt() {
    cerr << "PLAY SOUND_SALMONELLA_HURT" << endl;
    getStudentWorld()->playSound(SOUND_SALMONELLA_HURT);
}

void Salmonella::uniqueOnDeath() {
    cerr << "PLAY SOUND_SALMONELLA_DIE" << endl;
    getStudentWorld()->playSound(SOUND_SALMONELLA_DIE);
}

void Salmonella::move() {
    if (m_movementPlanDistance > 0) {
        m_movementPlanDistance -= 1;
        if (tryMove(getDirection(), 3)) {
            return;
        } else {
            setDirection(randInt(0, 359));
            m_movementPlanDistance = 10;
        }
    } else {
        double newX = getX(), newY = getY();
        if (getStudentWorld()->findClosestFood(newX, newY, 128, this)) {
            Direction newDir = getStudentWorld()->directionTo(getX(), getY(), newX, newY);
            if (!tryMove(newDir, 3)) {
                setDirection(randInt(0, 359));
                m_movementPlanDistance = 10;
            }
        } else {
            setDirection(randInt(0, 359));
            m_movementPlanDistance = 10;
        }
    }
}

bool Salmonella::aggr() {
    return false;
}

void Salmonella::onOverlap() {
    getStudentWorld()->hurtSoc(1);
}

void Salmonella::spawnNew(double x, double y) {
    Actor* temp = new Salmonella(x, y, getStudentWorld());
    getStudentWorld()->addActor(temp);
}

AggressiveSalmonella::AggressiveSalmonella(double x, double y, StudentWorld* world)
    : Salmonella(x, y, world, 10) {
}

AggressiveSalmonella::~AggressiveSalmonella() {
}

bool AggressiveSalmonella::aggr() {
    if (getStudentWorld()->distToSoc(getX(), getY()) <= 72) {
        tryMove(getStudentWorld()->dirToSoc(getX(), getY()), 3);
        return true;
    }
    return false;
}

void AggressiveSalmonella::onOverlap() {
    getStudentWorld()->hurtSoc(2);
}

void AggressiveSalmonella::spawnNew(double x, double y) {
    getStudentWorld()->addActor(new AggressiveSalmonella(x, y, getStudentWorld()));
}

EColi::EColi(double startX, double startY, StudentWorld* world)
    : Bacteria(IID_ECOLI, startX, startY, world, 5) {
}

EColi::~EColi() {
}

void EColi::move() {
    if (getStudentWorld()->distToSoc(getX(), getY()) <= 256) {
        int theta = getStudentWorld()->dirToSoc(getX(), getY());
        for (int i = 0; i < 10 && !tryMove(theta, 2); i++, theta += 10)
            if (theta + 10 >= 360)
                theta -= 360;
    }
}

void EColi::onHurt() {
    cerr << "PLAY SOUND_ECOLI_HURT" << endl;
    getStudentWorld()->playSound(SOUND_ECOLI_HURT);
}

void EColi::uniqueOnDeath() {
    cerr << "PLAY SOUND_ECOLI_DIE" << endl;
    getStudentWorld()->playSound(SOUND_ECOLI_DIE);
}

void EColi::onOverlap() {
    getStudentWorld()->hurtSoc(4);
}

void EColi::spawnNew(double x, double y) {
    getStudentWorld()->addActor(new EColi(x, y, getStudentWorld()));
}