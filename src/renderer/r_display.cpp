#include "../common.h"
#include "r_display.h"

static int displayWidth = 0;
static int displayHeight = 0;

static int latchedDisplayWidth = 0;
static int latchedDisplayHeight = 0;

static ALLEGRO_DISPLAY *gameDisplay = 0;

ALLEGRO_DISPLAY *R_InitDisplay(int defWidth, int defHeight)
{
	displayWidth = defWidth;
	displayHeight = defHeight;

	gameDisplay = al_create_display(displayWidth, displayHeight);

	return gameDisplay;
}

ALLEGRO_DISPLAY *R_GetDisplay() { return gameDisplay; }

void R_ShutdownDisplay()
{
	al_destroy_display(gameDisplay);
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
	al_resize_display(gameDisplay, displayWidth, displayHeight);
	return true;
}