#ifndef PLAYER__H
#define PLAYER__H

#include "twoWayMultisprite.h"

class Player : public TwoWayMultisprite {
public:
  Player(const std::string&, const std::string&);
  Player(const Player&);
  virtual void update(Uint32 ticks);

  void collided() { collision = true; }
  void missed() { collision = false; }
  Player& operator=(const Player&);

  void right();
  void left();
  void up();
  void down();
  void stop();

private:
  bool collision;
  Vector2f initialVelocity;
  bool jumpFlag;
  int jumpInc;

};
#endif
