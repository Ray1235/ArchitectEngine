#include "common.h"

bool AssetDB_Init()
{
	g_AssetList = (std::vector<A_Asset *> *)calloc(ASSET_TYPE_MAX, sizeof(std::vector<A_Asset *>));
	if (!g_AssetList)
	{
		Com_Error(ERR_FATAL, "Failed to initialize g_AssetList", "");
	}
	return true;
}
