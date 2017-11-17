#include "../common.h"
#include "r_display.h"

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

}

void R_BeginFrame()
{
	al_clear_to_color(al_map_rgba(255, 128, 0, 255));
}

void R_EndFrame()
{
	al_flip_display();
}