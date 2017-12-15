#include "enemySprite.h"
#include "gamedata.h"

EnemySprite::EnemySprite(const std::string& name) :
  MultiSprite(name),
  bulletName( Gamedata::getInstance().getXmlStr(name+"/bullet") ),
  bullets(),
  minSpeed( Gamedata::getInstance().getXmlInt(bulletName+"/speedX") ),
  bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/interval")),
  timeSinceLastFrame(0),
  freeList(),
  bulletsize(0),
  freesize(0),
  explosionFlag(false)
{ }

EnemySprite::EnemySprite(const EnemySprite& s) :
  MultiSprite(s),
  bulletName(s.bulletName),
  bullets(s.bullets),
  minSpeed(s.minSpeed),
  bulletInterval(s.bulletInterval),
  timeSinceLastFrame(s.timeSinceLastFrame),
  freeList(s.freeList),
  bulletsize( s.bulletsize),
  freesize( s.freesize),
  explosionFlag( s.explosionFlag )
{ }

void EnemySprite::shoot() { 
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

void EnemySprite::shootRight() {
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


void EnemySprite::update(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  MultiSprite::update(ticks);
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

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > 290) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 5000) {
    setVelocityX( fabs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -fabs( getVelocityX() ) );
  }  
}

void EnemySprite::draw() const { 
  MultiSprite::draw();
  for ( const Bullet& bullet : bullets ) {
    bullet.draw();
  }
}

