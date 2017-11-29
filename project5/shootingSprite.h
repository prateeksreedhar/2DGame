#ifndef SHOOTINGSPRITE__H
#define SHOOTINGSPRITE__H
#include <string>
#include <iostream>
#include <list>
#include "sprite.h"
#include "bullet.h"
#include "player.h"

class ShootingSprite : public Player {
public:
  ShootingSprite(const std::string& n);
  ShootingSprite(const ShootingSprite& s);

  virtual void update(Uint32 ticks);
  virtual void draw() const;
  void shoot();
  void shootRight();

private:
  std::string bulletName;
  std::list<Bullet> bullets;
  float minSpeed;
  float bulletInterval;
  float timeSinceLastFrame;
  std::list<Bullet> freeList;
  ShootingSprite& operator=(const ShootingSprite&);
};
#endif