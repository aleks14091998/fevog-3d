#ifndef _GAME_H
#define _GAME_H

#include"FEV_Eng.h"

class Editor {
private:
	valk::Engine *engine;
	valk::IInput *Input;
	valk::IRenderAPI *Render;
	valk::IWindowManager *windowManager;
public:
	Editor();
	~Editor();

	void init();
	void process();
	void render();
	void destroy();

	inline valk::IInput *&GetInput(){ return Input; }
	inline valk::IRenderAPI *&GetRender() { return Render; }
};

#endif