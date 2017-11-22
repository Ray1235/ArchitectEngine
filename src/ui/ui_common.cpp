#include "../common.h"
#include "../renderer/r_display.h"
#include "ui_imgui.h"

static int targetRes[2];

static int currentAssetListType = 0;

bool UI_Init()
{
	if(!UI_ImGui_Init(R_GetDisplay()))
	{
		return false;
	}
	targetRes[0] = 800;
	targetRes[1] = 600;
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
	ImGui::SetNextWindowPos(ImVec2(2, 2));
	ImGui::Begin("Performance", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_ShowBorders);
	ImGui::Text("%.1f FPS (%.3f ms/frame)",
		ImGui::GetIO().Framerate,
		1000.0f / ImGui::GetIO().Framerate);
	ImGui::End();

	if (ImGui::Begin("Assets"))
	{
		ImGui::Combo("Type", &currentAssetListType, AssetDB_ImGui_GetTypes, NULL, ASSET_TYPE_MAX + 1);
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
								if (((A_Material *)asset)->isEmissive)
									ImGui::Text("Emissive map: %s", ((A_Material *)asset)->emissive->name);
							}
							if (0) // damn, this is broken as hell //if (ImGui::Button(va("Unload asset##assetl_%d_%d", currentAssetListType - 1, i)))
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
	}
	ImGui::End();

	if (ImGui::Begin("Main")) /* Main game debug window */
	{
		if (ImGui::Button("Close game"))
		{
			shouldClose = true;
		}

		ImGui::Separator();

		if (ImGui::CollapsingHeader("Error Test"))
		{
			if (ImGui::Button("Normal Error"))
				Com_Error(ERR_NONE, "Normal %s test", "error");

			ImGui::SameLine();

			if (ImGui::Button("Fatal Error"))
				Com_Error(ERR_FATAL, "Fatal %s test", "error");

			ImGui::SameLine();

			if (ImGui::Button("Drop Error"))
				Com_Error(ERR_DROP, "Drop %s test", "error");
		}

		ImGui::Separator(); /* Resolution */

		ImGui::InputInt2("Resolution", targetRes);
		if (ImGui::Button("Apply"))
		{
			R_SetDisplayRes(targetRes[0], targetRes[1]);
			R_RefreshDisplay();
		}
	}
	ImGui::End();
}

void UI_EndFrame()
{
	UI_ImGui_EndFrame();
}

void UI_ProcessEvent(ALLEGRO_EVENT* event)
{
	UI_ImGui_ProcessEvent(event);
}