#include "common.h"

bool shouldClose = false;

int main(int argc, char *argv[])
{
	Com_Init();

	while(!shouldClose)
	{
		R_BeginFrame();
		UI_BeginFrame();

		Com_Frame();

		UI_EndFrame();
		R_EndFrame();
	}

	UI_Shutdown();
	R_Shutdown();
}