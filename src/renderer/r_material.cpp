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
	char *xml_content = (char *)malloc(al_fsize(f)+1);
	al_fread(f, xml_content, al_fsize(f));
	xml_document<> mat;
	mat.parse<0>(xml_content);
	
	xml_node<> *matN = mat.first_node("material");
	xml_node<> *colorN = matN->first_node("color");
	xml_node<> *emissiveN = matN->first_node("emissive");
	if (colorN)
	{
		color = al_load_bitmap(colorN->value());
	}
	if (emissiveN)
	{
		isEmissive = true;
		emissive = al_load_bitmap(emissiveN->value());
	}
	mat.clear();
	free(xml_content);
	isLoaded = true;
}

void A_Material::Unload()
{
	if(color) al_destroy_bitmap(color);
	if(emissive) al_destroy_bitmap(emissive);
	isLoaded = false;
}

bool R_MaterialSystem_Init()
{
	//R_PrecacheMaterial("default");
	return true;
}

int R_GetMaterialCount()
{
	return 0;
}

A_Material * R_PrecacheMaterial(char * name)
{
	A_Material * mat = new A_Material();
	mat->name = (char *)malloc(strlen(name) + 1);
	strcpy(mat->name, name);
	mat->Precache();
	AssetDB_AddAsset<ASSET_TYPE_MATERIAL>(mat);
	return mat;
}

void R_UnloadMaterial(A_Material * material)
{
	material->Unload();
}
