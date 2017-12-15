#ifndef TWOWAYMULTISPRITE__H
#define TWOWAYMULTISPRITE__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"


#include "explodingSprite.h"

class TwoWayMultisprite : public Drawable {
public:
  TwoWayMultisprite(const std::string&,const std::string&);
  TwoWayMultisprite(const TwoWayMultisprite&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual ~TwoWayMultisprite();
  virtual const Image* getImage() const { 
    return images[currentFrame]; 
  }
  int getScaledWidth()  const { 
    return getScale()*images[currentFrame]->getWidth(); 
  } 
  int getScaledHeight()  const { 
    return getScale()*images[currentFrame]->getHeight(); 
  } 
  virtual const SDL_Surface* getSurface() const { 
    return images[currentFrame]->getSurface();
  }
  virtual void explode();
  
  //bool shootright;

  void setShootleft() { shootleft=1;  }
  void unsetShootleft() { shootleft=0; }
private:


protected:
  std::vector<Image *> images;
  std::vector<Image *> images2;
  std::vector<Image *> images3;
  ExplodingSprite* explosion;
  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;
  bool shootleft;
  void advanceFrame(Uint32 ticks);
  TwoWayMultisprite& operator=(const TwoWayMultisprite&);
};
#endif