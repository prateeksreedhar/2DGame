#include "shootingSprite.h"
#include "gamedata.h"

ShootingSprite::ShootingSprite(const std::string& name) :
  Player(name, name+"Reverse"),
  bulletName( Gamedata::getInstance().getXmlStr(name+"/bullet") ),
  bullets(),
  minSpeed( Gamedata::getInstance().getXmlInt(bulletName+"/speedX") ),
  bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/interval")),
  timeSinceLastFrame(0),
  freeList()
{ }

ShootingSprite::ShootingSprite(const ShootingSprite& s) :
  Player(s),
  bulletName(s.bulletName),
  bullets(s.bullets),
  minSpeed(s.minSpeed),
  bulletInterval(s.bulletInterval),
  timeSinceLastFrame(s.timeSinceLastFrame),
  freeList(s.freeList)
{ }

void ShootingSprite::shoot() { 
  if ( timeSinceLastFrame < bulletInterval ) return;
  float deltaX = getScaledWidth()-300;
  float deltaY = getScaledHeight()/2 - 50;
  // I need to add some minSpeed to velocity:
if(freeList.empty())
  {
    std::cout << "COMING HERE" << std::endl;
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
        //    std::cout << b.getPosition() <<std::endl;

  }
}

void ShootingSprite::shootRight() {
  if ( timeSinceLastFrame < bulletInterval ) return;
  float deltaX = getScaledWidth();
  float deltaY = getScaledHeight()-130;
  // I need to add some minSpeed to velocity:
  if(freeList.empty())
  {
    std::cout << "COMING HERE" << std::endl;
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
        //    std::cout << b.getPosition() <<std::endl;

  }
          //  std::cout << "BULLETSSSSS : "<< b.getPosition();
}


void ShootingSprite::update(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  Player::update(ticks);
  std::list<Bullet>::iterator ptr=bullets.begin();
  while(ptr!=bullets.end())
  {
    ptr->update(ticks);
    //std::cout<< ptr->getX() << std::endl;
    //pos=ptr->getX();
    if(ptr->goneTooFar()){
      freeList.push_back(*ptr);
      ptr=bullets.erase(ptr);
    }
    else ++ptr;

// for ( Bullet& bullet : bullets ) {
//    bullet.update(ticks);
//  }
  }

  std::cout << "BULLETLIST"<< bullets.size() <<std::endl;
    std::cout << "FREELIST" << freeList.size() << std::endl;
 
}

void ShootingSprite::draw() const { 
  Player::draw();
  for ( const Bullet& bullet : bullets ) {
    bullet.draw();
  }
}

