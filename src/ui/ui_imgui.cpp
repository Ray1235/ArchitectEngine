#include <stdint.h>     // uint64_t
#include <cstring>      // memcpy
#include <cstdio>
#include "../common.h"
#include "imgui/imgui.h"
#include "ui_imgui.h"

#include <SFML/OpenGL.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>

#include <cmath> // abs
#include <cstddef> // offsetof, NULL
#include <cassert>
#include <SFML/Window/Touch.hpp>

static bool s_windowHasFocus = true;
static bool s_mousePressed[3] = { false, false, false };
static bool s_touchDown[3] = { false, false, false };
static bool s_mouseMoved = false;
static sf::Vector2i s_touchPos;
static sf::Texture* s_fontTexture = NULL; // owning pointer to internal font atlas which is used if user doesn't set custom sf::Texture.

void UI_ImGui_RenderDrawLists(ImDrawData* draw_data)
{

	if (draw_data->CmdListsCount == 0) {
		return;
	}

	ImGuiIO& io = ImGui::GetIO();
	assert(io.Fonts->TexID != NULL); // You forgot to create and set font texture

									 // scale stuff (needed for proper handling of window resize)
	int fb_width = static_cast<int>(io.DisplaySize.x * io.DisplayFramebufferScale.x);
	int fb_height = static_cast<int>(io.DisplaySize.y * io.DisplayFramebufferScale.y);
	if (fb_width == 0 || fb_height == 0) { return; }
	draw_data->ScaleClipRects(io.DisplayFramebufferScale);



#ifdef GL_VERSION_ES_CL_1_1
	GLint last_program, last_texture, last_array_buffer, last_element_array_buffer;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);
#else
	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TRANSFORM_BIT);
#endif

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

#ifdef GL_VERSION_ES_CL_1_1
	glOrthof(0.0f, io.DisplaySize.x, io.DisplaySize.y, 0.0f, -1.0f, +1.0f);
#else
	glOrtho(0.0f, io.DisplaySize.x, io.DisplaySize.y, 0.0f, -1.0f, +1.0f);
#endif

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	for (int n = 0; n < draw_data->CmdListsCount; ++n) {
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		const unsigned char* vtx_buffer = (const unsigned char*)&cmd_list->VtxBuffer.front();
		const ImDrawIdx* idx_buffer = &cmd_list->IdxBuffer.front();

		glVertexPointer(2, GL_FLOAT, sizeof(ImDrawVert), (void*)(vtx_buffer + offsetof(ImDrawVert, pos)));
		glTexCoordPointer(2, GL_FLOAT, sizeof(ImDrawVert), (void*)(vtx_buffer + offsetof(ImDrawVert, uv)));
		glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImDrawVert), (void*)(vtx_buffer + offsetof(ImDrawVert, col)));

		for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.size(); ++cmd_i) {
			const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
			if (pcmd->UserCallback) {
				pcmd->UserCallback(cmd_list, pcmd);
			}
			else {
				GLuint tex_id = (GLuint)*((unsigned int*)&pcmd->TextureId);
				glBindTexture(GL_TEXTURE_2D, tex_id);
				glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w),
					(int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
				glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, GL_UNSIGNED_SHORT, idx_buffer);
			}
			idx_buffer += pcmd->ElemCount;
		}
	}
#ifdef GL_VERSION_ES_CL_1_1
	glBindTexture(GL_TEXTURE_2D, last_texture);
	glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
	glDisable(GL_SCISSOR_TEST);
#else
	glPopAttrib();
#endif
}


bool UI_ImGui_Init(sf::RenderTarget *renderTarget, bool loadDefaultFont)
{
	ImGuiIO& io = ImGui::GetIO();

	// init keyboard mapping
	io.KeyMap[ImGuiKey_Tab] = sf::Keyboard::Tab;
	io.KeyMap[ImGuiKey_LeftArrow] = sf::Keyboard::Left;
	io.KeyMap[ImGuiKey_RightArrow] = sf::Keyboard::Right;
	io.KeyMap[ImGuiKey_UpArrow] = sf::Keyboard::Up;
	io.KeyMap[ImGuiKey_DownArrow] = sf::Keyboard::Down;
	io.KeyMap[ImGuiKey_PageUp] = sf::Keyboard::PageUp;
	io.KeyMap[ImGuiKey_PageDown] = sf::Keyboard::PageDown;
	io.KeyMap[ImGuiKey_Home] = sf::Keyboard::Home;
	io.KeyMap[ImGuiKey_End] = sf::Keyboard::End;
#ifdef ANDROID
	io.KeyMap[ImGuiKey_Backspace] = sf::Keyboard::Delete;
#else
	io.KeyMap[ImGuiKey_Delete] = sf::Keyboard::Delete;
	io.KeyMap[ImGuiKey_Backspace] = sf::Keyboard::BackSpace;
#endif
	io.KeyMap[ImGuiKey_Enter] = sf::Keyboard::Return;
	io.KeyMap[ImGuiKey_Escape] = sf::Keyboard::Escape;
	io.KeyMap[ImGuiKey_A] = sf::Keyboard::A;
	io.KeyMap[ImGuiKey_C] = sf::Keyboard::C;
	io.KeyMap[ImGuiKey_V] = sf::Keyboard::V;
	io.KeyMap[ImGuiKey_X] = sf::Keyboard::X;
	io.KeyMap[ImGuiKey_Y] = sf::Keyboard::Y;
	io.KeyMap[ImGuiKey_Z] = sf::Keyboard::Z;

	// init rendering
	io.DisplaySize = static_cast<sf::Vector2f>(renderTarget->getSize());
	io.RenderDrawListsFn = UI_ImGui_RenderDrawLists; // set render callback

	if (s_fontTexture) { // delete previously created texture
		delete s_fontTexture;
	}
	s_fontTexture = new sf::Texture;

	if (loadDefaultFont) {
		// this will load default font automatically
		// No need to call AddDefaultFont
		UI_ImGui_UpdateFontTexture();
	}
    return true;
}

void UI_ImGui_Shutdown()
{
	ImGui::GetIO().Fonts->TexID = NULL;

	if (s_fontTexture) { // if internal texture was created, we delete it
		delete s_fontTexture;
		s_fontTexture = NULL;
	}

	ImGui::Shutdown(); // need to specify namespace here, otherwise ImGui::SFML::Shutdown would be called
}

// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
bool UI_ImGui_ProcessEvent(const sf::Event& ev)
{
	ImGuiIO& io = ImGui::GetIO();
	if (s_windowHasFocus) {
		switch (ev.type)
		{
		case sf::Event::MouseMoved:
			s_mouseMoved = true;
			break;
		case sf::Event::MouseButtonPressed: // fall-through
		case sf::Event::MouseButtonReleased:
		{
			int button = ev.mouseButton.button;
			if (ev.type == sf::Event::MouseButtonPressed &&
				button >= 0 && button < 3) {
				s_mousePressed[ev.mouseButton.button] = true;
			}
		}
		break;
		case sf::Event::TouchBegan: // fall-through
		case sf::Event::TouchEnded:
		{
			s_mouseMoved = false;
			int button = ev.touch.finger;
			if (ev.type == sf::Event::TouchBegan &&
				button >= 0 && button < 3) {
				s_touchDown[ev.touch.finger] = true;
			}
		}
		break;
		case sf::Event::MouseWheelMoved:
			io.MouseWheel += static_cast<float>(ev.mouseWheel.delta);
			break;
		case sf::Event::KeyPressed: // fall-through
		case sf::Event::KeyReleased:
			io.KeysDown[ev.key.code] = (ev.type == sf::Event::KeyPressed);
			io.KeyCtrl = ev.key.control;
			io.KeyShift = ev.key.shift;
			io.KeyAlt = ev.key.alt;
			break;
		case sf::Event::TextEntered:
			if (ev.text.unicode > 0 && ev.text.unicode < 0x10000) {
				io.AddInputCharacter(static_cast<ImWchar>(ev.text.unicode));
			}
			break;
		default:
			break;
		}
	}

	switch (ev.type)
	{
	case sf::Event::LostFocus:
		s_windowHasFocus = false;
		break;
	case sf::Event::GainedFocus:
		s_windowHasFocus = true;
		break;
	default:
		break;
	}
	return true;
}

void UI_ImGui_NewFrame(sf::RenderWindow *window, sf::Time dt)
{
	UI_ImGui_NewFrame(window, window, dt);
}

void UI_ImGui_NewFrame(sf::Window *window, sf::RenderTarget *target, sf::Time dt)
{

	if (!s_mouseMoved)
	{
		if (sf::Touch::isDown(0))
			s_touchPos = sf::Touch::getPosition(0, *window);

		UI_ImGui_NewFrame(s_touchPos, static_cast<sf::Vector2f>(target->getSize()), dt);
	}
	else {
		UI_ImGui_NewFrame(sf::Mouse::getPosition(*window), static_cast<sf::Vector2f>(target->getSize()), dt);
	}
	window->setMouseCursorVisible(!ImGui::GetIO().MouseDrawCursor); // don't draw mouse cursor if ImGui draws it
}

void UI_ImGui_NewFrame(const sf::Vector2i& mousePos, const sf::Vector2f& displaySize, sf::Time dt)
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = displaySize;
	io.DeltaTime = dt.asSeconds();

	if (s_windowHasFocus) {
		io.MousePos = mousePos;
		for (unsigned int i = 0; i < 3; i++) {
			io.MouseDown[i] = s_touchDown[i] || sf::Touch::isDown(i) || s_mousePressed[i] || sf::Mouse::isButtonPressed((sf::Mouse::Button)i);
			s_mousePressed[i] = false;
			s_touchDown[i] = false;
		}
	}

	assert(io.Fonts->Fonts.Size > 0); // You forgot to create and set up font atlas (see createFontTexture)
	ImGui::NewFrame();
}

void UI_ImGui_EndFrame(sf::RenderTarget* target)
{
	target->resetGLStates();
    ImGui::Render();
}

void UI_ImGui_UpdateFontTexture()
{
	sf::Texture& texture = *s_fontTexture;

	ImGuiIO& io = ImGui::GetIO();
	unsigned char* pixels;
	int width, height;

	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

	texture.create(width, height);
	texture.update(pixels);

	io.Fonts->TexID = (void*)texture.getNativeHandle();

	io.Fonts->ClearInputData();
	io.Fonts->ClearTexData();
}