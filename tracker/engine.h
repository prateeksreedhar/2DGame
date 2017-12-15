#ifndef ENGINE__H
#define ENGINE__H
#include <vector>
#include <SDL.h>
#include "world.h"
#include "viewport.h"
#include "player.h"
#include "hud.h"
#include "shootingSprite.h"
#include "enemySprite.h"
#include "sound.h"
#include <SDL.h>

#include "collisionStrategy.h"
#include "smartSprite.h"
#include "subjectSprite.h"

class Engine {
public:
  Engine ();
  ~Engine ();
  bool play();
  void switchSprite();
  void initializeSprites();
  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  void printScales();
private:
  const RenderContext* rc;
  const IOmod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  World layer1;
  World layer2;
  World layer3;
  World layer4;
  World layer5;
  World layer6;
  World layer7;
  World layer8;

  Viewport& viewport;
  unsigned int currentSprite;
  bool makeVideo;
  int currentStrategy;
  Hud& hud;
  std::vector<CollisionStrategy*> strategies;
  SubjectSprite* player;
  EnemySprite* player2;
  std::vector<SmartSprite*> sprites;
  bool collision;
  void draw() const;
  void update(Uint32);

  void printScales() const;
  void checkForCollisions();
  void checkforEnemyCollision();
  bool hudFlag;
  bool shootFlag;
  SDLSound sound;
  bool godMode;
  int healthCount;
  int enemyHealthCount;
  std::vector<Drawable*> painter;
  int delay;
  bool enemyDead;
};

#endif