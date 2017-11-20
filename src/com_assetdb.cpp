#include "common.h"

std::vector<A_Asset *> g_AssetList[ASSET_TYPE_MAX];

bool AssetDB_Init()
{
	/*
	g_AssetList = (std::vector<A_Asset *> *)calloc(ASSET_TYPE_MAX, sizeof(std::vector<A_Asset *>));
	if (!g_AssetList)
	{
		Com_Error(ERR_FATAL, "Failed to initialize g_AssetList", "");
	}
	*/
	return true;
}

void AssetDB_Shutdown()
{
	for (int i = 0; i < ASSET_TYPE_MAX; i++)
	{
		for (int o = 0; o < g_AssetList[i].size(); o++)
		{
			g_AssetList[i][o]->Unload();
			free(g_AssetList[i][o]);
		}
	}
}

void AssetDB_AddAsset(int type, A_Asset * asset)
{
	for(int i = 0; i < g_AssetList[type].size(); i++)
	{
		A_Asset * a = g_AssetList[type][i];
		if (a == asset) return; // avoid duplicates
	}
	g_AssetList[type].push_back(asset);
}

int AssetDB_GetAssetCount(int type)
{
	int res = 0;
	for (int i = 0; i < g_AssetList[type].size(); i++)
	{
		if (g_AssetList[type][i]->isLoaded) res++;
	}
	return res;
}

A_Asset * AssetDB_GetAsset(int type, int index)
{
	assert(type < ASSET_TYPE_MAX && type >= 0);
	assert(index < g_AssetList[type].size());
	assert(index >= 0);
	assert(g_AssetList[type][index] != 0);
	return g_AssetList[type][index];
}

A_Asset * AssetDB_GetAssetByName(int type, char * name)
{
	for (int i = 0; i < g_AssetList[type].size(); i++)
	{
		if (strcmp(g_AssetList[type][i]->name, name) == 0)
		{
			return g_AssetList[type][i];
		}
	}
	return nullptr;
}

bool AssetDB_ImGui_GetTypes(void * ptr, int index, const char ** output)
{
	if (index > 0 && index <= ASSET_TYPE_MAX)
		*output = DB_AssetTypes[index-1].name;
	else if (index == 0)
		*output = "Show All";
	else
		return false;
	return true;
}
