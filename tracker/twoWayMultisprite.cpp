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

TwoWayMultisprite::TwoWayMultisprite( const std::string& nameForward, const std::string& nameReverse) :
  Drawable(nameForward, 
           Vector2f(Gamedata::getInstance().getXmlInt(nameForward+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(nameForward+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(nameForward+"/speedX"),
                    Gamedata::getInstance().getXmlInt(nameForward+"/speedY"))
           ),
  imagesForwardMotion( RenderContext::getInstance()->getImages(nameForward) ),
  imagesReverseMotion( RenderContext::getInstance()->getImages(nameReverse) ),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(nameForward+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(nameForward+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

TwoWayMultisprite::TwoWayMultisprite(const TwoWayMultisprite& s) :
  Drawable(s), 
  imagesForwardMotion(s.imagesForwardMotion),
  imagesReverseMotion(s.imagesReverseMotion),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight )
  { }

TwoWayMultisprite& TwoWayMultisprite::operator=(const TwoWayMultisprite& s) {
  Drawable::operator=(s);
  imagesForwardMotion = (s.imagesForwardMotion);
  imagesReverseMotion = (s.imagesReverseMotion);
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  return *this;
} 

void TwoWayMultisprite::draw() const { 
  static bool sideChange = false;
  const Uint8* keystate;
  keystate = SDL_GetKeyboardState(NULL);
  if(keystate[SDL_SCANCODE_A]){
    sideChange =  true;
  }
  if(keystate[SDL_SCANCODE_D]){
    sideChange =  false;
  }
  if(sideChange){
    imagesReverseMotion[currentFrame]->draw(getX(), getY(), getScale());
  }else{
    imagesForwardMotion[currentFrame]->draw(getX(), getY(), getScale());
  }
}

void TwoWayMultisprite::update(Uint32 ticks) { 
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
