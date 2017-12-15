#include "shootingSprite.h"
#include "gamedata.h"

ShootingSprite::ShootingSprite(const std::string& name) :
  Player(name, name+"Reverse"),
  bulletName( Gamedata::getInstance().getXmlStr(name+"/bullet") ),
  bullets(),
  minSpeed( Gamedata::getInstance().getXmlInt(bulletName+"/speedX") ),
  bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/interval")),
  timeSinceLastFrame(0),
  freeList(),
   bulletsize(0),
  freesize(0)
{ }

ShootingSprite::ShootingSprite(const ShootingSprite& s) :
  Player(s),
  bulletName(s.bulletName),
  bullets(s.bullets),
  minSpeed(s.minSpeed),
  bulletInterval(s.bulletInterval),
  timeSinceLastFrame(s.timeSinceLastFrame),
  freeList(s.freeList),
  bulletsize( s.bulletsize),
  freesize( s.freesize)
{ }

void ShootingSprite::shoot() { 
  if ( timeSinceLastFrame < bulletInterval ) return;
  float deltaX = getScaledWidth()-300;
  float deltaY = getScaledHeight()/2 ;
  // I need to add some minSpeed to velocity:
if(freeList.empty())
  {
    Bullet bullet(bulletName);
    bullet.setPosition( getPosition() + Vector2f(deltaX, deltaY) );
    bullet.setVelocity( getVelocity() - Vector2f(minSpeed, 0) );
    bullets.push_back( bullet );
    timeSinceLastFrame = 0;

  }
  else
  {

    Bullet b=freeList.front();
    freeList.pop_front();
    b.reset();
    b.setPosition( getPosition() +Vector2f(deltaX, deltaY) );
    b.setVelocity( getVelocity() - Vector2f(minSpeed, 0) );
    bullets.push_back(b);
  }
}

void ShootingSprite::shootRight() {
  if ( timeSinceLastFrame < bulletInterval ) return;
  float deltaX = getScaledWidth();
  float deltaY = getScaledHeight()/2;
  // I need to add some minSpeed to velocity:
  if(freeList.empty())
  {
    Bullet bullet(bulletName);
    bullet.setPosition( getPosition() +Vector2f(deltaX, deltaY) );
    bullet.setVelocity( getVelocity() + Vector2f(minSpeed, 0) );
    bullets.push_back( bullet );
    timeSinceLastFrame = 0;

  }
  else
  {

    Bullet b=freeList.front();
    freeList.pop_front();
    b.reset();
    b.setPosition( getPosition() +Vector2f(deltaX, deltaY) );
    b.setVelocity( getVelocity() + Vector2f(minSpeed, 0) );
    bullets.push_back(b);
  }
}


void ShootingSprite::update(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  Player::update(ticks);
  std::list<Bullet>::iterator ptr=bullets.begin();
  while(ptr!=bullets.end())
  {
    ptr->update(ticks);
    if(ptr->goneTooFar()){
      freeList.push_back(*ptr);
      ptr=bullets.erase(ptr);
    }
    else ++ptr;
  }
    bulletsize=bullets.size();
  freesize = freeList.size();
}

void ShootingSprite::draw() const { 
  Player::draw();
  for ( const Bullet& bullet : bullets ) {
    bullet.draw();
  }
}

