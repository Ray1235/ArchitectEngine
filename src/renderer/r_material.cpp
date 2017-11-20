#include "../common.h"

using namespace rapidxml;

void A_Material::Precache()
{
	ALLEGRO_FILE * f;
	f = al_fopen(va("%s%s", MATERIAL_PATH, name), "rb");
	if (f == NULL)
	{
		Com_Error(ERR_FATAL, "Tried to load a non-existent material %s", this->name);
	}
	int fsize = al_fsize(f);
	char *xml_content = (char *)calloc(al_fsize(f)+1, sizeof(char));
	al_fread(f, xml_content, al_fsize(f));
	xml_document<> mat;
	mat.parse<0>(xml_content);
	
	xml_node<> *matN = mat.first_node("material");
	if (!matN)
	{
		Com_Error(ERR_NONE, "Material %s exists, but is invalid", this->name);
		mat.clear();
		free(xml_content);
		return;
	}
	xml_node<> *colorN = matN->first_node("color");
	xml_node<> *emissiveN = matN->first_node("emissive");
	if (colorN && colorN->value())
	{
		//color = al_load_bitmap(colorN->value());
		A_Image * colorI = new A_Image();
		colorI->name = new char[strlen(colorN->value()) + 1];
		strcpy(colorI->name, colorN->value());
		colorI->Precache();
		AssetDB_AddAsset(ASSET_TYPE_IMAGE, colorI);
		this->color = colorI;
	}
	if (emissiveN && emissiveN->value())
	{
		isEmissive = true;
		//emissive = al_load_bitmap(emissiveN->value());
		A_Image * emissiveI = new A_Image();
		emissiveI->name = new char[strlen(emissiveN->value()) + 1];
		strcpy(emissiveI->name, emissiveN->value());
		emissiveI->Precache();
		AssetDB_AddAsset(ASSET_TYPE_IMAGE, emissiveI);
		this->emissive = emissiveI;
	}
	mat.clear();
	free(xml_content);
	isLoaded = true;
}

void A_Material::Unload()
{
	//if(color) al_destroy_bitmap(color);
	//if(emissive) al_destroy_bitmap(emissive);
	isLoaded = false;
}

bool R_MaterialSystem_Init()
{
	//R_PrecacheMaterial("default");
	R_CreateDefaultMaterial();
	R_PrecacheMaterial("engine_logo");
	return true;
}

int R_GetMaterialCount()
{
	return 0;
}

void R_CreateDefaultMaterial()
{
	A_Material * defaultMat = new A_Material();
	A_Image * defaultTex = new A_Image();
	ALLEGRO_BITMAP * defaultTexBitmap = al_create_bitmap(32, 32);
	ALLEGRO_BITMAP * originalTarget = al_get_target_bitmap();
	al_set_target_bitmap(defaultTexBitmap);
	for (int y = 0; y < 32; y++)
	{
		for (int x = 0; x < 32; x++)
		{
			al_put_pixel(x, y, ((x % 2) + (y % 2)) % 2 ? al_map_rgba(255, 64, 255, 255) : al_map_rgba(64, 255, 64, 255) );
		}
	}
	al_set_target_bitmap(originalTarget);

	defaultTex->bitmap = defaultTexBitmap;
	defaultTex->name = "default";
	defaultTex->isLoaded = true;

	defaultMat->color = defaultTex;
	defaultMat->name = "default";
	defaultMat->emissive = 0;
	defaultMat->isLoaded = true;

	AssetDB_AddAsset(ASSET_TYPE_IMAGE, defaultTex);
	AssetDB_AddAsset(ASSET_TYPE_MATERIAL, defaultMat);
}

A_Material * R_PrecacheMaterial(char * name)
{
	A_Material * mat = new A_Material();
	mat->name = (char *)malloc(strlen(name) + 1);
	strcpy(mat->name, name);
	mat->Precache();
	AssetDB_AddAsset(ASSET_TYPE_MATERIAL, mat);
	return mat;
}

void R_UnloadMaterial(A_Material * material)
{
	material->Unload();
}
