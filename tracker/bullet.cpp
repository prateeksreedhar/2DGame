#include <iostream>
#include <cmath>
#include "bullet.h"
void Bullet::update(Uint32 ticks) { 
  Vector2f pos = getPosition();
  Sprite::update(ticks);
  distance += ( hypot(getX()-pos[0], getY()-pos[1]) );
  int width = Gamedata::getInstance().getXmlInt("world/width");
  if (distance > maxDistance ||(getX()>=(width-100)) || (getX() <0 ))  tooFar = true;
}

