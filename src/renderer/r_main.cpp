#include "../common.h"
#include "r_display.h"
#include <SFML\OpenGL.hpp>

bool R_Init()
{
	if(!R_InitDisplay(640, 480))
	{
		return false;
	}

	if (!R_MaterialSystem_Init())
	{
		return false;
	}
	return true;
}

void R_Shutdown()
{
	R_ShutdownDisplay();
}

void R_BeginFrame()
{
	glClear(GL_COLOR_BUFFER_BIT);	
	//al_clear_to_color(al_map_rgba(0, 0, 0, 255));
}

void R_EndFrame()
{
	R_GetDisplay()->display();
	//al_flip_display();
}