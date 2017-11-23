#include "common.h"

bool shouldClose = false;

sf::Clock g_mainDTClock;
sf::Clock g_mainTClock;
sf::Time g_deltaTime;

int main(int argc, char *argv[])
{
	g_mainDTClock.restart();
	Com_Init();
	while(!shouldClose)
	{
		R_BeginFrame();
		UI_BeginFrame();

		Com_Frame();
		R_Frame();
		UI_Frame();

		UI_EndFrame();
		R_EndFrame();
		g_deltaTime = g_mainDTClock.restart();
	}

	Com_Shutdown();
	return 0;
}