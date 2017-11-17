#pragma once

#include <stdio.h>
#include <assert.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>

#include "com_main.h"

#include "renderer/r_main.h"
#include "renderer/r_display.h"

#include "ui/imgui/imgui.h"
#include "ui/ui_common.h"
#include "ui/ui_imgui.h"

extern bool shouldClose;

extern ALLEGRO_EVENT_QUEUE* g_EventQueue;