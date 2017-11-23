#pragma once

bool R_Init();

void R_Shutdown();

void R_BeginFrame();
void R_Frame();
void R_EndFrame();

#define SHADER_PATH "main/shaders/"