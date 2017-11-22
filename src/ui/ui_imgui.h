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

IMGUI_API bool    	UI_ImGui_Init(sf::RenderTarget *renderTarget, bool loadDefaultFont = true);
IMGUI_API void    	UI_ImGui_Shutdown();
IMGUI_API void    	UI_ImGui_NewFrame(sf::RenderWindow *window, sf::Time dt);
IMGUI_API void    	UI_ImGui_NewFrame(sf::Window *window, sf::RenderTarget *target, sf::Time dt);
IMGUI_API void    	UI_ImGui_NewFrame(const sf::Vector2i& mousePos, const sf::Vector2f& displaySize, sf::Time dt);
IMGUI_API void 		UI_ImGui_EndFrame(sf::RenderTarget* target);
IMGUI_API bool    	UI_ImGui_ProcessEvent(const sf::Event& ev);

IMGUI_API void		UI_ImGui_UpdateFontTexture();