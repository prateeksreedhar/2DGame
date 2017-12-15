#include "player.h"

Player::Player( const std::string& name, const std::string& name1) :
  TwoWayMultisprite(name, name1),
  collision(false),
  initialVelocity(getVelocity()),
  jumpFlag(false),
  jumpInc(0)

{ }

Player::Player(const Player& s) :
  TwoWayMultisprite(s), 
  collision(s.collision),
  initialVelocity(s.getVelocity()),
  jumpFlag(s.jumpFlag),
  jumpInc(s.jumpInc)
  { }

Player& Player::operator=(const Player& s) {
  TwoWayMultisprite::operator=(s);
  collision = s.collision;
  initialVelocity = s.initialVelocity;
  jumpFlag = s.jumpFlag;
  jumpInc = s.jumpInc;
  return *this;
}

void Player::stop() { 
  setVelocity(Vector2f(0,0));
}

void Player::right() { 
    TwoWayMultisprite::unsetShootleft();

  if ( getX() < worldWidth-getScaledWidth()) {
    setVelocityX(initialVelocity[0]);
  }
} 
void Player::left()  { 
  TwoWayMultisprite::setShootleft();
  if ( getX() > 0) {
    setVelocityX(-initialVelocity[0]);
  }
} 
void Player::up()  { 
  if ( getY() > 0 ){
    //setVelocityY( -initialVelocity[1] );
      jumpFlag = true;

  }
} 
void Player::down()  { 
  if ( getY() < worldHeight-getScaledHeight()) {
    setVelocityY( initialVelocity[1] );
  }
}

void Player::update(Uint32 ticks) {
  TwoWayMultisprite::update(ticks);




  if(getVelocityX()!=0)
  {
    advanceFrame(ticks);

  }
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.0015;
  setPosition(getPosition() + incr);

  stop();


  if(jumpFlag){

    if(jumpInc>=0&&jumpInc<=10){
            jumpInc += 1;

      setY(getY()-10);
      setVelocityY( -initialVelocity[1]-10 );
    }else if(jumpInc>=10&&jumpInc<=20){
      jumpInc += 1;
      setY(getY()+10);
      setVelocityY( -initialVelocity[1]+10 );
    }

    if(jumpInc>20){
      jumpFlag = false;
      jumpInc = 0;
      setY(250);
    } 
  }else jumpInc=0;
}