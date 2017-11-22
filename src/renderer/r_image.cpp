#include "../common.h"

void A_Image::Precache()
{
	this->image = new sf::Image();
	if(this->image->loadFromFile(va("%s%s.tga", IMAGE_PATH, name)))
	{
		this->gpuTexture = new sf::Texture();
		this->gpuTexture->loadFromImage(*this->image);
		isLoaded = true;
	}
}

void A_Image::Unload()
{
	//delete this->gpuTexture;
	//delete this->image;
	isLoaded = false;
}
