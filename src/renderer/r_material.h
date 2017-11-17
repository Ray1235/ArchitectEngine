#pragma once
#include "../common.h"
#include <allegro5/allegro.h>

#define MATERIAL_PATH "materials/"
#define IMAGE_PATH "images/"

class A_Material : public A_Asset {
public:
	char * name;
	bool isEmissive;
	ALLEGRO_BITMAP * color;
	ALLEGRO_BITMAP * emissive;

	void Precache();
	void Unload();

};

bool R_MaterialSystem_Init();
int R_GetMaterialCount();

A_Material * R_PrecacheMaterial(char * name);
void R_UnloadMaterial(A_Material * material);