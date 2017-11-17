#include <vector>
#include <SDL.h>
#include "world.h"
#include "viewport.h"
#include "player.h"
#include "hud.h"
class CollisionStrategy;
class SmartSprite;
class SubjectSprite;

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
  unsigned int currentSprite;
  bool makeVideo;
  int currentStrategy;
  Hud& hud;
  std::vector<CollisionStrategy*> strategies;
  SubjectSprite* player;
  std::vector<SmartSprite*> sprites;
  bool collision;
  std::vector<Drawable*> normalSprites;
  void draw() const;
  void update(Uint32);

  void printScales() const;
  void checkForCollisions();
  bool hudFlag;
};
