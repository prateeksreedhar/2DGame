#include "hud.h"
Hud::Hud():
rc( RenderContext::getInstance() ),
io(IOmod::getInstance()),
renderer( rc->getRenderer() ),
keystate(SDL_GetKeyboardState(NULL)),
gamedata(Gamedata::getInstance())
{

}

Hud::~Hud(){
	std::cout << "deleting hud" << std::endl;
}

Hud& Hud::getInstance(){
	static Hud instance;
	return instance;
}
void Hud::draw(){
	SDL_Rect hudBorder;
	hudBorder.x = gamedata.getXmlInt("hud/xpos");
	hudBorder.y = gamedata.getXmlInt("hud/ypos");
	hudBorder.w = gamedata.getXmlInt("hud/width");
	hudBorder.h = gamedata.getXmlInt("hud/height");

	SDL_Color bgColor;
	bgColor.r = gamedata.getXmlInt("hudColor/r");
	bgColor.g = gamedata.getXmlInt("hudColor/g");
	bgColor.b = gamedata.getXmlInt("hudColor/b");
	bgColor.a = gamedata.getXmlInt("hudColor/a");

	SDL_Color hudText;
	hudText.r = gamedata.getXmlInt("hud/r");
	hudText.g = gamedata.getXmlInt("hud/g");
	hudText.b = gamedata.getXmlInt("hud/b");
	hudText.a = gamedata.getXmlInt("hud/a");

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
	SDL_RenderFillRect( renderer, &hudBorder );
	io.writeText(gamedata.getXmlStr("hud/text").c_str(), hudBorder.x+=6, hudBorder.y+=22, hudText);
	io.writeText(gamedata.getXmlStr("hud/left").c_str(), hudBorder.x, hudBorder.y+=30, hudText);
	io.writeText(gamedata.getXmlStr("hud/right").c_str(), hudBorder.x, hudBorder.y+=30, hudText);
	io.writeText(gamedata.getXmlStr("hud/up").c_str(), hudBorder.x, hudBorder.y+=30, hudText);
	io.writeText(gamedata.getXmlStr("hud/down").c_str(), hudBorder.x, hudBorder.y+=30, hudText);
}