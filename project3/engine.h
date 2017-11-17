#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "player.h"

class Engine {
public:
  Engine ();
  ~Engine ();
  void play();
  void switchSprite();
  void initializeSprites();
  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
private:
  const RenderContext* rc;
  const IOmod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  World sky;
  World rocks;
  World clouds;
  World ground;
  Viewport& viewport;
  std::vector<Drawable*> sprites;
  unsigned int currentSprite;

  bool makeVideo;
  Player* player;
  int currentStrategy;
  //std::vector<CollisionStrategy*> strategies;
  //bool collision;
  void draw() const;
  void update(Uint32);

  void printScales() const;
  void checkForCollisions();
};
