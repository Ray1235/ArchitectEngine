#pragma once
#include "../common.h"
//#include <allegro5/allegro.h>
#include <SFML\Graphics.hpp>

#define IMAGE_PATH "main/images/"

class A_Image : public A_Asset {
public:
	sf::Image *image;
	sf::Texture *gpuTexture;

	bool useMipMaps = false;

	void Precache();
	void Unload();

};