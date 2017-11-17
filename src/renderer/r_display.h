#pragma once

ALLEGRO_DISPLAY *R_InitDisplay(int w, int h);
ALLEGRO_DISPLAY *R_GetDisplay();
void R_ShutdownDisplay();

int R_GetDisplayWidth();
int R_GetDisplayHeight();

void R_SetDisplayRes(int w, int h);

bool R_RefreshDisplay();