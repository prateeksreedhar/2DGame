#ifndef TWOWAYMULTISPRITE__H
#define TWOWAYMULTISPRITE__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class TwoWayMultisprite : public Drawable {
public:
  TwoWayMultisprite(const std::string&, const std::string&);
  TwoWayMultisprite(const TwoWayMultisprite&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { 
    return imagesForwardMotion[currentFrame]; 
  }
  int getScaledWidth()  const { 
    return getScale()*imagesForwardMotion[currentFrame]->getWidth(); 
  } 
  int getScaledHeight()  const { 
    return getScale()*imagesForwardMotion[currentFrame]->getHeight(); 
  } 
  virtual const SDL_Surface* getSurface() const { 
    return imagesForwardMotion[currentFrame]->getSurface();
  }

protected:
  std::vector<Image *> imagesForwardMotion;
  std::vector<Image *> imagesReverseMotion;
  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  void advanceFrame(Uint32 ticks);
  TwoWayMultisprite& operator=(const TwoWayMultisprite&);
};
#endif
