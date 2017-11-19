#pragma once
#include "../common.h"
#include <allegro5/allegro.h>

#define IMAGE_PATH "main/images/"

class A_Image : public A_Asset {
public:
	ALLEGRO_BITMAP * bitmap;

	void Precache();
	void Unload();

};