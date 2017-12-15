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
#include <SDL.h>

Engine::~Engine() {
  delete player;
  delete player2;
  for(auto sprite : sprites){
    delete sprite;
  }
  
  for ( CollisionStrategy* strategy : strategies ) {
    delete strategy;
  }
  for(auto paint : painter){
    delete paint;
  }
  std::cout << "Terminating program" << std::endl;
}

class SpriteLess {
public:
  bool operator()(const Drawable* lhs, const Drawable* rhs) const {
    return lhs->getScale() < rhs->getScale();
  }
};

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IOmod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  layer1("layer1", Gamedata::getInstance().getXmlInt("layer1/factor")),
  layer2("layer2", Gamedata::getInstance().getXmlInt("layer2/factor")),
  layer3("layer3", Gamedata::getInstance().getXmlInt("layer3/factor")),
  layer4("layer4", Gamedata::getInstance().getXmlInt("layer4/factor")),
  layer5("layer5", Gamedata::getInstance().getXmlInt("layer5/factor")),
  layer6("layer6", Gamedata::getInstance().getXmlInt("layer6/factor")),
  layer7("layer7", Gamedata::getInstance().getXmlInt("layer7/factor")),
  layer8("layer8", Gamedata::getInstance().getXmlInt("layer8/factor")),
  viewport( Viewport::getInstance() ),
  currentSprite(0),
  makeVideo( false ),
  currentStrategy(0),
  hud(Hud::getInstance()),
  strategies(),
  player(new SubjectSprite("sasukeRun")),
  player2(new EnemySprite("sasuRunReverse")),
  sprites(),
  collision(false),
  hudFlag(false),
  shootFlag(false),
  sound(),
  godMode(false),
  healthCount(100),
  enemyHealthCount(100),
  painter(),
  delay(0),
  enemyDead(false)
{
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

  unsigned int numberForPainter = Gamedata::getInstance().getXmlInt("numberForPainter");
  for ( unsigned int i = 0; i < numberForPainter; ++i ) {
    Drawable* s = new Sprite("Aeroplane");
    float scale = Gamedata::getInstance().getRandFloat(0.05,0.2);
    s->setScale(scale);
    painter.push_back(s);
  }
  std::vector<Drawable*>::iterator ptr = painter.begin();
  ++ptr;
  sort(ptr, painter.end(), SpriteLess());
}

void Engine::printScales() const {
  for (Drawable* s : sprites) {
    std::cout << s->getScale() << std::endl;
  }
}

void Engine::draw() const {
  layer8.draw();
  for(unsigned int i=0;i<painter.size()/3;i++){
    painter[i]->draw();
  }
  layer7.draw();
  for(unsigned int i=(painter.size()/3)+1;i<painter.size()*2/3;i++){
    painter[i]->draw();
  }
  layer6.draw();
  for(unsigned int i=(painter.size()*2/3)+1;i<painter.size();i++){
    painter[i]->draw();
  }
  layer5.draw();
  layer4.draw();
  layer3.draw();
  layer2.draw();
  layer1.draw();
  std::stringstream bulletlist; 
  bulletlist<< player->getBulletListSize();
  std::stringstream freelist; 
  freelist<< player->getFreeListSize();
  std::string bl="BULLETLIST: "+bulletlist.str();
  std::string fl="FREELIST: "+freelist.str();
  
  for(auto sprite : sprites){
    sprite->draw();
  }
  std::stringstream strm;
  strm << sprites.size() << " Smart Sprites Remaining";
  //IOmod::getInstance().writeText(strm.str(), 600, 95, {100, 250, 250, 0});
  //strategies[currentStrategy]->draw();
  player->draw();
  if(player2->getExplosionFlag()==false && player2!=NULL){
    player2->draw();
  }
  viewport.draw();
  std::stringstream str;
  str << "Fps: " << clock.getFps();
  if(clock.getSeconds() <= 3 || hudFlag){
    hud.draw();
    hud.drawLists(bl,fl);
  }
  //Health of my player

  IOmod::getInstance().writeText("Player Health", 619, 49, {0, 0, 0, 255});
  const SDL_Rect pBorder = {619, 79, 102, 22};
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderDrawRect(renderer,&pBorder);
  if(healthCount>0){
  const SDL_Rect q = {620, 80, healthCount, 20};
  SDL_RenderDrawRect(renderer, &q);
  if(healthCount>50){
    SDL_SetRenderDrawColor(renderer, 0, 253, 0, 255);
  }else{
    SDL_SetRenderDrawColor(renderer, 253, 0, 0, 255);
  }
  SDL_RenderFillRect(renderer, &q);
    
}

  //Health of enemy

  IOmod::getInstance().writeText("Enemy Health", 619, 119, {0, 0, 0, 255});
  const SDL_Rect eBorder = {619, 149, 102, 22};
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderDrawRect(renderer,&eBorder);
  if(enemyHealthCount>0){
  const SDL_Rect r = {620, 150, enemyHealthCount, 20};
  SDL_RenderDrawRect(renderer, &r);
  if(enemyHealthCount>50){
    SDL_SetRenderDrawColor(renderer, 255, 0, 200, 255);
  }else{
    SDL_SetRenderDrawColor(renderer, 255, 200, 255, 255);
  }
  SDL_RenderFillRect(renderer, &r);
  }
  IOmod::getInstance().writeText("Prateek Sreedhar Bharadwaj", 30, 440, {0, 255, 0, 0});

  //Condition for game conclusion

  if(healthCount <= 0){
  IOmod::getInstance().writeText("GAME OVER.. PRESS R TO RESTART", 300, 100, {255, 255, 0, 0});
    clock.pause();
  }

  if(enemyHealthCount <=0){
  IOmod::getInstance().writeText("You Win.. PRESS R TO RESTART", 300, 100, {255, 255, 0, 0});
    clock.pause();
  }

  if(godMode){
    IOmod::getInstance().writeText("GOD MODE", 619, 19, {100, 250, 250, 0});
  }
  
    SDL_RenderPresent(renderer);
}

void Engine::checkforEnemyCollision(){
  collision = false;
  std::list<Bullet> bulletlist = player2->getBulletList();
  for ( const Bullet d : bulletlist ) {
    //Check for enemy bullet or enemy

    if ( strategies[currentStrategy]->execute(*player, d) || strategies[currentStrategy]->execute(*player, *player2)) {
      collision = true;
      if(!godMode){
        healthCount-=1;
        if(healthCount<2){
          player->explode();
        }
      }
      sound[0];
    }
  }

    if(enemyHealthCount<1){
      enemyDead = true;
      if(player2!=NULL){
        std::cout<<"Explode sasu";
        player2->explode();
      }
    }

  if(player2!=NULL&&player2->getExplosionFlag()){
    delete player2;
    player2 = NULL;
    return;
  }
}
void Engine::checkForCollisions() {
  collision = false;
  std::list<Bullet> bulletlist = player->getBulletList();
  for ( const Drawable* d : sprites ) {
    if ( strategies[currentStrategy]->execute(*player, *d) ) {
      collision = true;
      if(!godMode){
        healthCount-=1;
        if(healthCount<2){
          player->explode();
        }
      }
      sound[0];
    }
  }

  for(const auto d : bulletlist)
  {
  std::vector<SmartSprite*>::iterator it = sprites.begin();
    while(it !=sprites.end()){
      if(strategies[currentStrategy]->execute(**it, d)){
        (*it)->explode();
      }
      ++it;
    }
    if(strategies[currentStrategy]->execute(*player2, d)){
      enemyHealthCount-=1;
    }
  }
  std::vector<SmartSprite*>::iterator it1 = sprites.begin();
  while(it1 !=sprites.end()){
      if((*it1)->getExplode()){
        SmartSprite* doa = *it1;
        player->detach(doa);
        delete doa;
        it1 = sprites.erase(it1);
      }else{
        ++it1;
      }
      
    }

  if ( collision ) {
    player->collided();
  }
  else {
    player->missed();
    collision = false;
  }

}

void Engine::update(Uint32 ticks) {
  checkForCollisions();
  checkforEnemyCollision();
  player->update(ticks);
  if(player2!=NULL)
  {player2->update(ticks);}

  if(delay <= 10){
    delay++;
  }else{
    delay = 0;
    if(player2!=NULL)
    {player2->shoot();}
  }
  
  for(auto sprite : sprites){
    sprite->update(ticks);
  }
 
  //for(auto* s : painter) s->update(ticks);
  for(unsigned int i=0;i<painter.size()/3;i++){
    painter[i]->update(ticks);
  }
  for(unsigned int i=(painter.size()/3)+1;i<painter.size()*2/3;i++){
    painter[i]->update(ticks);
  }
  for(unsigned int i=(painter.size()*2/3)+1;i<painter.size();i++){
    painter[i]->update(ticks);
  }
  layer8.update();
  layer7.update();
  layer6.update();
  layer5.update();
  layer4.update();
  layer3.update();
  layer2.update();
  layer1.update();
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



bool Engine::play() {
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
        if ( keystate[SDL_SCANCODE_R] ) {
          clock.unpause();
          return true;
        }
        if( keystate[SDL_SCANCODE_G] ){
          if(godMode){
            godMode = false;
          }else{
            godMode = true;
          }
        }
         if ( keystate[SDL_SCANCODE_SPACE] ) {
          sound[1];
          if(shootFlag){
            static_cast<ShootingSprite*>(player)->shoot();
          }else{
          static_cast<ShootingSprite*>(player)->shootRight();
          }
        }
        if( keystate[SDL_SCANCODE_F1]){
          if(hudFlag) hudFlag = false;
          else hudFlag = true;
        }
        if( keystate[SDL_SCANCODE_E] ){
          player->explode();
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
        shootFlag = true;
        player->left();
      }
      if (keystate[SDL_SCANCODE_D]) {
        shootFlag = false;
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
  return false;
}