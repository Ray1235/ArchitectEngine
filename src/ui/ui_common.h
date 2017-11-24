#pragma once

bool UI_Init();

void UI_BeginFrame();
void UI_Frame();
void UI_EndFrame();

void UI_ProcessEvent(sf::Event &event);

void UI_Shutdown();