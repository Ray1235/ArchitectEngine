#pragma once
#include "../common.h"

#define MATERIAL_PATH "main/materials/"

class A_Material : public A_Asset {
public:
	bool isEmissive;
	A_Image * color;
	A_Image * emissive;

	void Precache();
	void Unload();

};

bool R_MaterialSystem_Init();
int R_GetMaterialCount();

void R_CreateDefaultMaterials();

extern A_Material * defaultMaterial;
extern A_Material * whiteMaterial;

A_Material * R_PrecacheMaterial(char * name);
void R_UnloadMaterial(A_Material * material);