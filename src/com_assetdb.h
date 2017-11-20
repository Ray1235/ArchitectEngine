#pragma once
#include "common.h"
#include <allegro5/allegro.h>

#define DEFINE_ASSET_TYPE(name, limit) {name, #name, limit}

class A_Asset {
public:

	virtual void Precache() = 0;
	virtual void Unload() = 0;
	
	bool isLoaded = false;

	char * name;
};

enum
{
	ASSET_TYPE_IMAGE,
	ASSET_TYPE_MATERIAL,
	ASSET_TYPE_MAX
};

typedef struct assetType_s {
	int type;
	char* name;
	int maxLimit;
} assetType_t;

static assetType_t DB_AssetTypes[] = {
	DEFINE_ASSET_TYPE(ASSET_TYPE_IMAGE, 4096),
	DEFINE_ASSET_TYPE(ASSET_TYPE_MATERIAL, 4096),
};

extern std::vector<A_Asset *> g_AssetList[ASSET_TYPE_MAX];

bool AssetDB_Init();

void AssetDB_Shutdown();

void AssetDB_AddAsset(int type, A_Asset * asset);
int AssetDB_GetAssetCount(int type);

A_Asset * AssetDB_GetAsset(int type, int index);
A_Asset * AssetDB_GetAssetByName(int type, char * name);

bool AssetDB_ImGui_GetTypes(void* ptr, int index, const char** output);
