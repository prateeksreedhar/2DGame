#ifndef ENEMYSPRITE__H
#define ENEMYSPRITE__H
#include <string>
#include <iostream>
#include <list>
#include "multisprite.h"
#include "bullet.h"
#include "player.h"

class EnemySprite : public MultiSprite {
public:
  EnemySprite(const std::string& n);
  EnemySprite(const EnemySprite& s);

  virtual void update(Uint32 ticks);
  virtual void draw() const;
  void shoot();
  void shootRight();
  std::list<Bullet> getBulletList() {return bullets;}
  int getBulletListSize() {return bulletsize; }
  int getFreeListSize() {return freesize;}

private:
  std::string bulletName;
  std::list<Bullet> bullets;
  float minSpeed;
  float bulletInterval;
  float timeSinceLastFrame;
  std::list<Bullet> freeList;
  EnemySprite& operator=(const EnemySprite&);
  int bulletsize;
  int freesize;
  bool explosionFlag;
};
#endif