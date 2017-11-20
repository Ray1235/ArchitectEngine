#include "../common.h"

void A_Image::Precache()
{
	this->bitmap = al_load_bitmap(va("%s%s.tga", IMAGE_PATH, this->name));
	if (this->bitmap)
	{
		isLoaded = true;
	}
}

void A_Image::Unload()
{
	al_destroy_bitmap(this->bitmap);
	isLoaded = false;
}
