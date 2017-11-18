#include "common.h"

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

void AssetDB_AddAsset(int type, A_Asset * asset)
{
	for(int i = 0; i < g_AssetList[type].size(); i++)
	{
		A_Asset * a = g_AssetList[type][i];
		if (a == asset) return; // avoid duplicates
	}
	g_AssetList[type].push_back(asset);
}
