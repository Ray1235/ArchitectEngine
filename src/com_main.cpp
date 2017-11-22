#include "common.h"
#include "com_discord.h"

#include <SFML/OpenGL.hpp>

void Com_Init()
{

	if(!AssetDB_Init())
	{
		Com_Error(ERR_FATAL, "Failed to initialize AssetDB", "");
	}

	if(!R_Init())
	{
		Com_Error(ERR_FATAL, "Failed to initialize the renderer", "");
	}

	if(!UI_Init())
	{
		Com_Error(ERR_FATAL, "Failed to initalize the UI System", "");
	}

	Com_Discord_Init();
}

float nextDiscordPresenceUpdate = 0.0f;

void Com_Frame()
{
	sf::Event ev;
	while (R_GetDisplay()->pollEvent(ev))
	{
		if (ev.type == sf::Event::Closed) shouldClose = true;
		UI_ImGui_ProcessEvent(ev);
	}

	if (nextDiscordPresenceUpdate < g_mainDTClock.getElapsedTime().asSeconds() )
	{
		nextDiscordPresenceUpdate = g_mainDTClock.getElapsedTime().asSeconds() + 1.0f;
		Com_Discord_UpdatePresence();
	}
}

void Com_Shutdown()
{
	Com_Discord_Shutdown();
	UI_Shutdown();
	R_Shutdown();
	AssetDB_Shutdown();
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
		/*
		al_show_native_message_box(
			R_GetDisplay(),
			"Error",
			source,
			msg,
			NULL,
			ALLEGRO_MESSAGEBOX_ERROR);
		*/
#ifdef _WIN32
		//MessageBox(R_GetDisplay()->getSystemHandle(), va("%s\n%s", source, msg), "Error", 0);
#endif
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