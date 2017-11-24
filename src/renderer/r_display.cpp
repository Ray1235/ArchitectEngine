#include "../common.h"
#include "r_display.h"

static int displayWidth = 0;
static int displayHeight = 0;

static int latchedDisplayWidth = 0;
static int latchedDisplayHeight = 0;

static sf::RenderWindow gameDisplay;

sf::RenderWindow *R_InitDisplay(int defWidth, int defHeight)
{
	displayWidth = defWidth;
	displayHeight = defHeight;
	
	gameDisplay.create(sf::VideoMode(displayWidth, displayHeight), "Architect", sf::Style::Default, sf::ContextSettings(24, 8, 4, 3, 3));

	return &gameDisplay;
}

sf::RenderWindow *R_GetDisplay() { return &gameDisplay; }

sf::RenderTarget * R_GetRenderTarget()
{
	return &gameDisplay;
}

void R_ShutdownDisplay()
{
	gameDisplay.close();
	//al_destroy_display(gameDisplay);
}

int R_GetDisplayWidth() { return displayWidth; }
int R_GetDisplayHeight() { return displayHeight; }

void R_SetDisplayRes(int w, int h)
{
	latchedDisplayWidth = w;
	latchedDisplayHeight = h;
}

bool R_RefreshDisplay()
{
	displayWidth = latchedDisplayWidth;
	displayHeight = latchedDisplayHeight;
	//al_resize_display(gameDisplay, displayWidth, displayHeight);
	gameDisplay.setSize(sf::Vector2u(displayWidth, displayHeight));
	return true;
}