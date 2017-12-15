#include "twoWayMultisprite.h"
#include "gamedata.h"
#include "renderContext.h"

void TwoWayMultisprite::advanceFrame(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
    timeSinceLastFrame = 0;
  }
}

TwoWayMultisprite::~TwoWayMultisprite( ) { 
  if (explosion) 
    delete explosion;
}


TwoWayMultisprite::TwoWayMultisprite( const std::string& name, const std::string& namerev) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images( RenderContext::getInstance()->getImages(name) ),
  images2( RenderContext::getInstance()->getImages(namerev) ),
  images3( RenderContext::getInstance()->getImages(name) ),
   explosion(nullptr),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  shootleft(0)
{ 

}

TwoWayMultisprite::TwoWayMultisprite(const TwoWayMultisprite& s) :
  Drawable(s), 
  images(s.images),
  images2(s.images2),
  images3(s.images3),
   explosion(s.explosion),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  shootleft(0)
  {

   }

TwoWayMultisprite& TwoWayMultisprite::operator=(const TwoWayMultisprite& s) {
  Drawable::operator=(s);
  images = (s.images);
  images2 = (s.images2);
  images3= (s.images3);
  explosion = s.explosion;
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  shootleft = (s.shootleft);
  return *this;
}

void TwoWayMultisprite::explode() {
  if ( !explosion ) {
    if(shootleft==0)
    {Sprite 
    sprite(getName(), getPosition(), getVelocity(), images[currentFrame]);
    explosion = new ExplodingSprite(sprite);}
    else
    {
      Sprite 
    sprite(getName(), getPosition(), getVelocity(), images2[currentFrame]);
    explosion = new ExplodingSprite(sprite);
    }
    
  }
 
}

void TwoWayMultisprite::draw() const { 
  const Uint8* keystate = SDL_GetKeyboardState(NULL);
 static int flag=0;
if ( explosion ) explosion->draw();

else
{

 if(keystate[SDL_SCANCODE_A])
 {
  flag=1;
 }

 if(keystate[SDL_SCANCODE_D])
 {
  flag=0;
 }

if(flag==0)
{
 images[currentFrame]->draw(getX(), getY(), getScale());
}

if(flag==1)
{
  images2[currentFrame]->draw(getX(), getY(), getScale());
} 
}
}



void TwoWayMultisprite::update(Uint32 ticks) {
    if ( explosion ) {
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      delete explosion;
      explosion = NULL;
    }
    return;
  }
 
    if(!(getVelocityX() == 0 && getVelocityY() == 0))   // If the player is not moving do not advance the frame i.e (getVeocity() == (0,0))
  {
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight()) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -fabs( getVelocityX() ) );
  } 
}
}