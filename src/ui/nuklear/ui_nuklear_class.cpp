#include "../../common.h"

#include "nuklear.h"

#include "ui_nuklear_class.h"

#include <SFML/Window.hpp>

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

NK_API struct nk_context*   nk_sfml_init(sf::Window* window);
NK_API void                 nk_sfml_font_stash_begin(struct nk_font_atlas** atlas);
NK_API void                 nk_sfml_font_stash_end(void);
NK_API int                  nk_sfml_handle_event(sf::Event* event);
NK_API void					nk_sfml_change_window(sf::Window* window);
NK_API void                 nk_sfml_render(enum nk_anti_aliasing, int max_vertex_buffer, int max_element_buffer);
NK_API void                 nk_sfml_shutdown(void);

NK_API void                 nk_sfml_device_create(void);
NK_API void                 nk_sfml_device_destroy(void);

static int targetRes[2];

static int currentAssetListType = 0;
static bool assetFilter[ASSET_TYPE_MAX];

extern struct nk_sfml sfml;

UI_Nuklear::UI_Nuklear()
{
	
}

UI_Nuklear::~UI_Nuklear()
{
}

struct nk_color background;

bool UI_Nuklear::Init(sf::Window* window)
{
	ctx = nk_sfml_init(window);

	struct nk_font_atlas *atlas;
	nk_sfml_font_stash_begin(&atlas);
	nk_sfml_font_stash_end();

	background = nk_rgb(28, 48, 62);

	targetRes[0] = 800;
	targetRes[1] = 600;

	return ctx != NULL;
}

void UI_Nuklear::Shutdown()
{
	nk_sfml_shutdown();
}

void UI_Nuklear::BeginFrame()
{
	nk_input_begin(ctx);
}

void UI_Nuklear::ProcessEvent(sf::Event *ev)
{
	nk_sfml_handle_event(ev);
}

void UI_Nuklear::EndFrame()
{
	nk_input_end(ctx);
}

void UI_Nuklear::Draw()
{
	if (nk_begin(ctx, "Main", nk_rect(50, 50, 230, 250),
		NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
		NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
	{
		enum { EASY, HARD };
		static int op = EASY;
		static int property = 20;

		nk_menubar_begin(ctx);
		nk_layout_row_begin(ctx, NK_STATIC, 15, 2);
		nk_layout_row_push(ctx, 45);
		if (nk_menu_begin_label(ctx, "FILE", NK_TEXT_LEFT, nk_vec2(120, 200))) {
			nk_layout_row_dynamic(ctx, 20, 1);
			nk_menu_item_label(ctx, "OPEN", NK_TEXT_LEFT);
			nk_menu_item_label(ctx, "CLOSE", NK_TEXT_LEFT);
			nk_menu_end(ctx);
		}
		nk_layout_row_push(ctx, 100);
		if (nk_menu_begin_label(ctx, "TEST ERROR", NK_TEXT_LEFT, nk_vec2(120, 200))) {
			nk_layout_row_dynamic(ctx, 20, 1);

#define ERROR_TEST(lvl) if(nk_menu_item_label(ctx, #lvl, NK_TEXT_LEFT)) { Com_Error(lvl, "%s", #lvl); }
			
			ERROR_TEST(ERR_NONE);
			ERROR_TEST(ERR_DROP);
			ERROR_TEST(ERR_FATAL);

			nk_menu_end(ctx);
		}
		nk_layout_row_end(ctx);
		nk_menubar_end(ctx);

		nk_layout_row_dynamic(ctx, 30, 1);
		if (nk_button_label(ctx, "Close game"))
		{
			shouldClose = true;
		}
			//fprintf(stdout, "button pressed\n");

		nk_layout_row_dynamic(ctx, 30, 3);
		nk_label(ctx, "Resolution", NK_TEXT_LEFT);
		nk_property_int(ctx, "X", 1, &targetRes[0], 3840, 1, 1.0f);
		nk_property_int(ctx, "Y", 1, &targetRes[1], 2160, 1, 1.0f);
		nk_layout_row_dynamic(ctx, 25, 1);
		if (nk_button_label(ctx, "Apply"))
		{
			R_SetDisplayRes(targetRes[0], targetRes[1]);
			R_RefreshDisplay();
		}

		nk_layout_row_dynamic(ctx, 25, 1);
		nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);

		nk_layout_row_dynamic(ctx, 20, 1);
		nk_label(ctx, "color:", NK_TEXT_LEFT);
		nk_layout_row_dynamic(ctx, 25, 1);

		if (nk_combo_begin_color(ctx, background, nk_vec2(nk_widget_width(ctx), 400))) {
			nk_layout_row_dynamic(ctx, 120, 1);
			background = nk_color_picker(ctx, background, NK_RGBA);
			nk_layout_row_dynamic(ctx, 25, 1);
			background.r = (nk_byte)nk_propertyi(ctx, "#R:", 0, background.r, 255, 1, 1);
			background.g = (nk_byte)nk_propertyi(ctx, "#G:", 0, background.g, 255, 1, 1);
			background.b = (nk_byte)nk_propertyi(ctx, "#B:", 0, background.b, 255, 1, 1);
			background.a = (nk_byte)nk_propertyi(ctx, "#A:", 0, background.a, 255, 1, 1);
			nk_combo_end(ctx);
		}
	}
	nk_end(ctx);


	if (nk_begin(ctx, "Assets", nk_rect(50, 50, 250, 500), NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
	{
		for (int i = 0; i < ASSET_TYPE_MAX; i++)
		{
			nk_layout_row_dynamic(ctx, 25, 1);
			assetFilter[i] = nk_check_label(ctx, DB_AssetTypes[i].name, assetFilter[i]);
			if (assetFilter[i])
			{
				for (int o = 0; o < g_AssetList[i].size(); o++)
				{
					A_Asset *asset = AssetDB_GetAsset(i, o);
					struct nk_image img;
					switch (i)
					{
					case ASSET_TYPE_IMAGE:
						nk_layout_row_template_begin(ctx, 64);
						nk_layout_row_template_push_static(ctx, 64);
						nk_layout_row_template_push_variable(ctx, 120);
						nk_layout_row_template_end(ctx);

						img.handle = nk_handle_ptr((void*)((A_Image *)asset)->gpuTexture->getNativeHandle());
						img.w = (unsigned short)((A_Image *)asset)->gpuTexture->getSize().x;
						img.h = (unsigned short)((A_Image *)asset)->gpuTexture->getSize().y;
						img.region[0] = 0;
						img.region[1] = 0;
						img.region[2] = (unsigned short)((A_Image *)asset)->gpuTexture->getSize().x;
						img.region[3] = (unsigned short)((A_Image *)asset)->gpuTexture->getSize().y;
						nk_image(ctx, img);
						nk_button_label(ctx, asset->name);
						break;
					default:
						nk_layout_row_dynamic(ctx, 15, 2);
						nk_label(ctx, asset->name, NK_TEXT_ALIGN_LEFT);
						nk_label(ctx, asset->isLoaded ? "" : "(not loaded)", NK_TEXT_ALIGN_RIGHT);
						break;
					}
				}
			}
		}
	}
	nk_end(ctx);
}

void UI_Nuklear::Render()
{
	nk_sfml_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
}
