// ImGui Allegro 5 bindings
// In this binding, ImTextureID is used to store a 'ALLEGRO_BITMAP*' texture identifier. Read the FAQ about ImTextureID in imgui.cpp.

// You can copy and use unmodified imgui_impl_* files in your project. See main.cpp for an example of using this.
// If you use this binding you'll need to call 4 functions: ImGui_ImplXXXX_Init(), ImGui_ImplXXXX_NewFrame(), ImGui::Render() and ImGui_ImplXXXX_Shutdown().
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.
// https://github.com/ocornut/imgui
// by @birthggd

#pragma once
#include "imgui/imgui.h"

struct ALLEGRO_DISPLAY;
union ALLEGRO_EVENT;

IMGUI_API bool    	UI_ImGui_Init(ALLEGRO_DISPLAY* display);
IMGUI_API void    	UI_ImGui_Shutdown();
IMGUI_API void    	UI_ImGui_NewFrame();
IMGUI_API void 		UI_ImGui_EndFrame();
IMGUI_API bool    	UI_ImGui_ProcessEvent(ALLEGRO_EVENT* event);

IMGUI_API bool    	UI_ImGui_CreateDeviceObjects();
IMGUI_API void    	UI_ImGui_InvalidateDeviceObjects();