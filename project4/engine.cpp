#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "smartSprite.h"
#include "subjectSprite.h"
#include "sprite.h"
#include "multisprite.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include "twoWayMultisprite.h"
#include "player.h"
#include "collisionStrategy.h"

Engine::~Engine() {
  delete player;
  for(auto sprite : sprites){
    delete sprite;
  }
  for(auto sprite : normalSprites){
    delete sprite;
  } 
  for ( CollisionStrategy* strategy : strategies ) {
    delete strategy;
  }
  std::cout << "Terminating program" << std::endl;
}

void Engine::initializeSprites(){
  normalSprites.push_back(new MultiSprite("shuriken"));
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IOmod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  sky("sky", Gamedata::getInstance().getXmlInt("sky/factor") ),
  rocks("rocks", Gamedata::getInstance().getXmlInt("rocks/factor") ),
  clouds("clouds", Gamedata::getInstance().getXmlInt("clouds/factor") ),
  ground("ground", Gamedata::getInstance().getXmlInt("ground/factor") ),
  viewport( Viewport::getInstance() ),
  currentSprite(0),
  makeVideo( false ),
  currentStrategy(0),
  hud(Hud::getInstance()),
  strategies(),
  player(new SubjectSprite("sasukeRun", "sasukeRunReverse")),
  sprites(),
  collision(false),
  normalSprites(),
  hudFlag(false)
  
{
  initializeSprites();
  int numberOfBalls = Gamedata::getInstance().getXmlInt("numberOfBalls");
  sprites.reserve(numberOfBalls);
  Vector2f pos = player->getPosition();
  int w = player->getScaledWidth();
  int h = player->getScaledHeight();
  for (int i = 0; i < numberOfBalls; ++i) {
    sprites.push_back( new SmartSprite("ball", pos, w, h) );
    player->attach( sprites[i] );
  }
  strategies.push_back( new RectangularCollisionStrategy );
  strategies.push_back( new PerPixelCollisionStrategy );
  strategies.push_back( new MidPointCollisionStrategy );
  Viewport::getInstance().setObjectToTrack(player);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  sky.draw();
  clouds.draw();
  rocks.draw();
  ground.draw();
  
  for(auto sprite : sprites){
    sprite->draw();
  }
  std::stringstream strm;
  strm << sprites.size() << " Smart Sprites Remaining";
  IOmod::getInstance().writeText(strm.str(), 580, 60, {100, 250, 250, 0});
  for(auto it : normalSprites){
    it->draw();
  }
  strategies[currentStrategy]->draw();
  if ( collision ) {
    IOmod::getInstance().writeText("Oops: Collision", 600, 40, {0,0,255,0});
  }
  player->draw();
  viewport.draw();
  std::stringstream str;
  str << "Fps: " << clock.getFps();
  if(clock.getSeconds() <= 3 || hudFlag){
    hud.draw();
  }
  IOmod::getInstance().writeText("Prateek Sreedhar Bharadwaj", 30, 440, {0, 255, 0, 0});
  SDL_RenderPresent(renderer);
}

void Engine::checkForCollisions() {
  collision = false;
  for ( const Drawable* d : sprites ) {
    if ( strategies[currentStrategy]->execute(*player, *d) ) {
      collision = true;
    }
  }
  for ( const Drawable* d : normalSprites ) {
    if ( strategies[currentStrategy]->execute(*player, *d) ) {
      collision = true;
    }
  }
  if ( collision ) {
    player->collided();
  }
  else {
    player->missed();
    collision = false;
  }
  auto it = sprites.begin();
  while ( it != sprites.end() ) {
    if ( strategies[currentStrategy]->execute(*player, **it) ) {
      SmartSprite* doa = *it;
      player->detach(doa);
      delete doa;
      it = sprites.erase(it);
    }
    else ++it;
  }
}

void Engine::update(Uint32 ticks) {
  checkForCollisions();
  player->update(ticks);
  for(auto sprite : sprites){
    sprite->update(ticks);
  }
  for(auto it: normalSprites){
    it->update(ticks);
  }
  sky.update();
  clouds.update();
  rocks.update();
  ground.update();
  viewport.update(); // always update viewport last
}

void Engine::switchSprite(){
  ++currentSprite;
  if ( currentSprite == sprites.size() ) {
    currentSprite = 0;
  }
  unsigned int count = 0;
  for(auto sprite : sprites){
    if(currentSprite == count){
      Viewport::getInstance().setObjectToTrack(sprite);
    }
    count++;
  }
}



void Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if( keystate[SDL_SCANCODE_F1]){
          if(hudFlag) hudFlag = false;
          else hudFlag = true;
        }
        if ( keystate[SDL_SCANCODE_M] ) {
          currentStrategy = (1 + currentStrategy) % strategies.size();
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    // In this section of the event loop we allow key bounce:
    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      if (keystate[SDL_SCANCODE_A]) {
        player->left();
      }
      if (keystate[SDL_SCANCODE_D]) {
        player->right();
      }
      if (keystate[SDL_SCANCODE_W]) {
        player->up();
      }
      if (keystate[SDL_SCANCODE_S]) {
        player->down();
      }
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
}
