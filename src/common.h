#pragma once

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>

#ifdef _M_AMD64
#include <discord-rpc.h>
#endif

#endif

#include <iostream>
#include <vector>

#include <stdio.h>
#include <assert.h>
/*
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
*/

#include <SFML\Main.hpp>
#include <SFML\Window.hpp>

//#include <allegro5/allegro_physfs.h>
//#include <physfs.h>
#include "rapidxml/rapidxml.hpp"

#include "com_main.h"
#include "com_assetdb.h"

#include "renderer/r_main.h"
#include "renderer/r_image.h"
#include "renderer/r_material.h"
#include "renderer/r_display.h"

#include "ui/imgui/imgui.h"
#include "ui/ui_common.h"
#include "ui/ui_imgui.h"

extern bool shouldClose;

extern sf::Clock g_mainDTClock;
extern sf::Clock g_mainTClock;
extern sf::Time g_deltaTime;
