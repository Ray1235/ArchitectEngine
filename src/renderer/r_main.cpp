#include "../common.h"
#include "r_display.h"

#include <allegro5\allegro_opengl.h>
#include <gl\GL.h>


static const GLfloat g_vertex_buffer_data[] = {
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	0.0f,  1.0f, 0.0f,
};

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
	al_clear_to_color(al_map_rgba(0, 0, 0, 255));
}

void R_EndFrame()
{
	al_flip_display();
}