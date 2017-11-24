#pragma once

#include "nuklear.h"

class UI_Nuklear
{
public:
	UI_Nuklear();
	~UI_Nuklear();

	bool Init(sf::Window* window);
	void Shutdown();

	void BeginFrame();
	void ProcessEvent(sf::Event *ev);
	void EndFrame();

	void Draw();
	void Render();

private:
	nk_context *ctx;
};