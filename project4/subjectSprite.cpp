#include "subjectSprite.h"
#include "smartSprite.h"
#include "gamedata.h"
#include "renderContext.h"

SubjectSprite::SubjectSprite( const std::string& name1, const std::string& name2) :
  Player(name1, name2), 
  observers()
{ }

SubjectSprite::SubjectSprite(const SubjectSprite& s) :
  Player(s), 
  observers( s.observers )
  { }

void SubjectSprite::detach( SmartSprite* o ) {
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    if ( *ptr == o ) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}

void SubjectSprite::update(Uint32 ticks) { 
  Player::update(ticks);
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    (*ptr)->setPlayerPos( getPosition() );
    ++ptr;
  }
}
