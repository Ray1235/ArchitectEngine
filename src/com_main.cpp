#include "common.h"
#include "com_discord.h"

ALLEGRO_EVENT_QUEUE* g_EventQueue;

void Com_Init()
{

	if(!al_init())
	{
		Com_Error(ERR_FATAL, "Failed to initialize Allegro 5", "");
	}

	if(!al_init_native_dialog_addon())
	{
		Com_Error(ERR_FATAL, "Failed to initialize Allegro 5 Native Dialog Addon", "");
	}

	//al_set_physfs_file_interface();
	//PHYSFS_mount("main", "", 0);

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

	if(!AssetDB_Init())
	{
		Com_Error(ERR_FATAL, "Failed to initialize AssetDB", "");
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

	Com_Discord_Init();
}

float nextDiscordPresenceUpdate = 0.0f;

void Com_Frame()
{
	ALLEGRO_EVENT ev;
	while (al_get_next_event(g_EventQueue, &ev))
	{
		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			shouldClose = true;
		}
		UI_ImGui_ProcessEvent(&ev);
	}
	

	if (nextDiscordPresenceUpdate < al_get_time())
	{
		nextDiscordPresenceUpdate = al_get_time() + 1.0f;
		Com_Discord_UpdatePresence();
	}

	//al_draw_bitmap(((A_Image *)g_AssetList[ASSET_TYPE_IMAGE][0])->bitmap, 32, 32, 0);
	//al_draw_bitmap_region(((A_Image *)g_AssetList[ASSET_TYPE_IMAGE][0])->bitmap, 0, 0, 32, 32, 512, 512, 0);
	//al_draw_scaled_bitmap(((A_Image *)g_AssetList[ASSET_TYPE_IMAGE][0])->bitmap, 0, 0, 32, 32, 0, 0, 512, 512, 0);
}

void Com_Shutdown()
{
	Com_Discord_Shutdown();
	UI_Shutdown();
	al_destroy_event_queue(g_EventQueue);
	R_Shutdown();
	AssetDB_Shutdown();

	al_uninstall_mouse();
	al_uninstall_keyboard();
	al_shutdown_primitives_addon();
	al_shutdown_image_addon();
	al_shutdown_native_dialog_addon();

	al_uninstall_system();
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