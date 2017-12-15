#include <SDL.h>
#include "ioMod.h"
#include "clock.h"
#include "renderContext.h"
#include "gamedata.h"
class Hud {
public:
	~Hud();
	void draw();
	void drawLists(const std::string&,const std::string&);
	

	static Hud& getInstance();
	Hud(const Hud& ) = delete;
	Hud& operator=(const Hud& ) = delete;
private:
	Hud();
	const RenderContext* rc;
	const IOmod& io;
	SDL_Renderer * const renderer;
	const Uint8* keystate;
	const Gamedata& gamedata;
};