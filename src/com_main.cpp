#include "common.h"

static int targetRes[2];
ALLEGRO_EVENT_QUEUE* g_EventQueue;

void Com_Init()
{
	targetRes[0] = 800;
	targetRes[1] = 600;

	if(!al_init())
	{
		Com_Error(ERR_FATAL, "Failed to initialize Allegro 5", "");
	}

	if(!al_init_native_dialog_addon())
	{
		Com_Error(ERR_FATAL, "Failed to initialize Allegro 5 Native Dialog Addon", "");
	}

	if(!al_init_image_addon())
	{
		Com_Error(ERR_FATAL, "Failed to initialize Allegro 5 Image Addon", "");
	}

	if(!al_init_primitives_addon())
	{
		Com_Error(ERR_FATAL, "Failed to initialize Allegro 5 Primitives Addon", "");
	}

	if(!al_install_keyboard())
	{
		Com_Error(ERR_FATAL, "Failed to initialize keyboard", "");
	}

	if(!al_install_mouse())
	{
		Com_Error(ERR_FATAL, "Failed to initialize mouse", "");
	}

	if(!R_Init())
	{
		Com_Error(ERR_FATAL, "Failed to initialize the renderer", "");
	}

	g_EventQueue = al_create_event_queue();

	if(!g_EventQueue)
	{
		Com_Error(ERR_FATAL, "Failed to create the event queue", "");
	}

    al_register_event_source(g_EventQueue, al_get_display_event_source(R_GetDisplay()));
    al_register_event_source(g_EventQueue, al_get_keyboard_event_source());
    al_register_event_source(g_EventQueue, al_get_mouse_event_source());

	if(!UI_Init())
	{
		Com_Error(ERR_FATAL, "Failed to initalize the UI System", "");
	}
}

void Com_Frame()
{
	ALLEGRO_EVENT ev;
	while (al_get_next_event(g_EventQueue, &ev))
	{
		UI_ImGui_ProcessEvent(&ev);
	}
	ImGui::SetNextWindowPos(ImVec2(2, 2));
	ImGui::Begin("Performance", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_ShowBorders);
	ImGui::Text("%.1f FPS (%.3f ms/frame)",
		ImGui::GetIO().Framerate,
		1000.0f / ImGui::GetIO().Framerate);
	ImGui::End();

	ImGui::Begin("Main"); /* Main game debug window */

	if(ImGui::Button("Close game"))
	{
		shouldClose = true;
	}

	ImGui::Separator();

	if(ImGui::CollapsingHeader("Error Test"))
	{
		if(ImGui::Button("Normal Error"))
			Com_Error(ERR_NONE, "Normal %s test", "error");

		ImGui::SameLine();

		if(ImGui::Button("Fatal Error"))
			Com_Error(ERR_FATAL, "Fatal %s test", "error");

		ImGui::SameLine();

		if(ImGui::Button("Drop Error"))
			Com_Error(ERR_DROP, "Drop %s test", "error");
	}

	ImGui::Separator(); /* Resolution */

	ImGui::InputInt2("Resolution", targetRes);
	if(ImGui::Button("Apply"))
	{
		R_SetDisplayRes(targetRes[0], targetRes[1]);
		R_RefreshDisplay();
	}

	ImGui::End();
}

void Com_Shutdown()
{

}

char * va(const char * fmt, ...)
{
	static int index = 0;
	static char string[4][1024];	// in case called by nested functions
	char *result;
	result = string[index];
	index = (index + 1) & 3;
	va_list args;
	va_start(args, fmt);
	vsprintf(result, fmt, args);
	va_end(args);
	return result;
}

void Com_Printf(char * fmt, ...)
{

}

void Com_ErrorEx(int level, char * source, char * msg)
{
	printf("ERROR: %s\n%s\n", source, msg);

	if (level > ERR_NONE)
	{
		al_show_native_message_box(
			R_GetDisplay(),
			"Error",
			source,
			msg,
			NULL,
			ALLEGRO_MESSAGEBOX_ERROR);
	}

#ifdef _WIN32
	if(IsDebuggerPresent())
	{
		if(level > ERR_NONE)
		{
			__debugbreak(); // if we're running with a debugger, break
		}
	}
#endif

	if(level == ERR_FATAL)
	{
		exit(-1);
	}

	if(level == ERR_DROP)
	{
		
	}
}