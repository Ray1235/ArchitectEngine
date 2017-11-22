#pragma once

sf::RenderWindow *R_InitDisplay(int w, int h);
sf::RenderWindow *R_GetDisplay();
sf::RenderTarget *R_GetRenderTarget();
void R_ShutdownDisplay();

int R_GetDisplayWidth();
int R_GetDisplayHeight();

void R_SetDisplayRes(int w, int h);

bool R_RefreshDisplay();