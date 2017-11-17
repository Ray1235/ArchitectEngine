#include "../common.h"
#include "../renderer/r_display.h"
#include "ui_imgui.h"

bool UI_Init()
{
	if(!UI_ImGui_Init(R_GetDisplay()))
	{
		return false;
	}
	return true;
}

void UI_Shutdown()
{
	UI_ImGui_Shutdown();
}

void UI_BeginFrame()
{
	UI_ImGui_NewFrame();
}

void UI_Frame()
{

}

void UI_EndFrame()
{
	UI_ImGui_EndFrame();
}

void UI_ProcessEvent(ALLEGRO_EVENT* event)
{
	UI_ImGui_ProcessEvent(event);
}