#include "common.h"
#include "com_discord.h"

static int targetRes[2];
ALLEGRO_EVENT_QUEUE* g_EventQueue;
static bool testWindow = true;

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

static int currentAssetListType = 0;

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
	ImGui::SetNextWindowPos(ImVec2(2, 2));
	ImGui::Begin("Performance", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_ShowBorders);
	ImGui::Text("%.1f FPS (%.3f ms/frame)",
		ImGui::GetIO().Framerate,
		1000.0f / ImGui::GetIO().Framerate);
	ImGui::End();

	ImGui::ShowTestWindow(&testWindow);

	ImGui::Begin("Assets");
	//ImGui::InputInt("Type", &currentAssetListType);
	const char* preview_text = NULL;
	ImGui::Combo("Type", &currentAssetListType, AssetDB_ImGui_GetTypes, NULL, ASSET_TYPE_MAX+1);
	if (currentAssetListType <= ASSET_TYPE_MAX && currentAssetListType > 0)
	{
		ImGui::LabelText(va("%d/%d", AssetDB_GetAssetCount(currentAssetListType - 1), DB_AssetTypes[currentAssetListType - 1].maxLimit), "Asset Count:");
		for (int i = 0; i < g_AssetList[currentAssetListType - 1].size(); i++)
		{
			A_Asset * asset;
			if ((asset = AssetDB_GetAsset(currentAssetListType - 1, i)) != NULL)
			{
				if (ImGui::CollapsingHeader(va("%s %s##asset_%d_%d", asset->name, asset->isLoaded ? "(loaded)" : "(unloaded)", currentAssetListType - 1, i)))
				{
					if (asset->isLoaded)
					{
						if (currentAssetListType - 1 == ASSET_TYPE_IMAGE)
						{
							if (((A_Image *)asset)->bitmap)
							{
								ImGui::Image(((A_Image *)asset)->bitmap, ImVec2(32, 32));
								if (ImGui::IsItemHovered())
								{
									ImGui::BeginTooltip();
									ImGui::Text("Size: %dx%d", al_get_bitmap_width(((A_Image *)asset)->bitmap), al_get_bitmap_height(((A_Image *)asset)->bitmap));
									ImGui::EndTooltip();
								}
							}
						}
						else if (currentAssetListType - 1 == ASSET_TYPE_MATERIAL)
						{
							ImGui::Text("Color map: %s", ((A_Material *)asset)->color->name);
							if(((A_Material *)asset)->isEmissive)
								ImGui::Text("Emissive map: %s", ((A_Material *)asset)->emissive->name);
						}
						if(0) // damn, this is broken as hell //if (ImGui::Button(va("Unload asset##assetl_%d_%d", currentAssetListType - 1, i)))
						{
							asset->Unload();
						}
					}
					else
					{
						ImGui::Text("This asset is currently unloaded, but you can force load it.");
						if (ImGui::Button(va("Force load##assetl_%d_%d", currentAssetListType - 1, i)))
						{
							asset->Precache();
						}
					}
				}
			}
		}
	}
	if (currentAssetListType == 0)
	{
		{
			for (int i = 0; i < ASSET_TYPE_MAX; i++)
			{
				if (ImGui::CollapsingHeader(va("%s##AssetI%d", DB_AssetTypes[i].name, i)))
				{
					ImGui::Indent();
					for (int o = 0; o < g_AssetList[i].size(); o++)
					{
						if (g_AssetList[i][o] != NULL) ImGui::Text(va("%s %s", g_AssetList[i][o]->name, g_AssetList[i][o]->isLoaded ? "(loaded)" : "(unloaded)"));
					}
					ImGui::Unindent();
				}
			}
		}
	}
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

	if (nextDiscordPresenceUpdate < al_get_time())
	{
		nextDiscordPresenceUpdate = al_get_time() + 1.0f;
		Com_Discord_UpdatePresence();
	}

	//al_draw_bitmap(((A_Image *)g_AssetList[ASSET_TYPE_IMAGE][0])->bitmap, 32, 32, 0);
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