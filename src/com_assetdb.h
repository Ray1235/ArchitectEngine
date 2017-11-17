#pragma once
#include "common.h"
#include <allegro5/allegro.h>

class A_Asset {
public:

	virtual void Precache() = 0;
	virtual void Unload() = 0;
	
	bool isLoaded = false;

	char * name;
};

enum
{
	ASSET_TYPE_MATERIAL,
	ASSET_TYPE_MAX
};

typedef struct assetType_s {
	int type;
	int maxLimit;
} assetType_t;

static assetType_t DB_AssetTypes[] = {
	{ASSET_TYPE_MATERIAL, 1024},
};

static std::vector<A_Asset *> * g_AssetList;

bool AssetDB_Init();

template <int type>
void AssetDB_AddAsset(A_Asset * asset);

template<int type>
inline void AssetDB_AddAsset(A_Asset * asset)
{
	for(int i = 0; i < g_AssetList[type].size(); i++)
	{
		A_Asset * a = g_AssetList[type][i];
		if (a == asset) return; // avoid duplicates
	}
	g_AssetList[type].push_back(asset);
}
