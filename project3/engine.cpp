#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include "twoWayMultisprite.h"
#include "player.h"

Engine::~Engine() {
  for(auto sprite : sprites){
    delete sprite;
  }
  std::cout << "Terminating program" << std::endl;
}

void Engine::initializeSprites(){
 // sprites.push_back(new Sprite("ball"));
 // sprites.push_back(new MultiSprite("shuriken"));
 // sprites.push_back(new TwoWayMultisprite("sasukeRun", "sasukeRunReverse"));
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
  player(new Player("sasukeRun", "sasukeRunReverse")),
  currentStrategy(0)
{
  initializeSprites();
  Viewport::getInstance().setObjectToTrack(player);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  sky.draw();
  clouds.draw();
  rocks.draw();
  ground.draw();
  player->draw();
  for(auto sprite : sprites){
    sprite->draw();
  }
  viewport.draw();
  std::stringstream str;
  str << "Fps: " << clock.getFps();
  IOmod::getInstance().writeText("Left->A", 30, 79, {255, 255, 0, 0});
  IOmod::getInstance().writeText("Right->D", 30, 109, {255, 255, 0, 0});
  
  IOmod::getInstance().writeText(str.str(), 30, 55, {255, 255, 0, 0});
  IOmod::getInstance().writeText("Prateek Sreedhar Bharadwaj", 30, 440, {0, 255, 0, 0});
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  for(auto sprite : sprites){
    sprite->update(ticks);
  }
  player->update(ticks);
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
        /*if ( keystate[SDL_SCANCODE_M] ) {
          currentStrategy = (1 + currentStrategy) % strategies.size();
        }*/
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
