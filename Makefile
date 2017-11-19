CC=g++
CFLAGS=-Wall -fpermissive

LIBS=
ALLEGRO_LIBS=allegro-5 allegro_acodec-5 allegro_audio-5 allegro_color-5 allegro_dialog-5 allegro_font-5 allegro_image-5 allegro_main-5 allegro_memfile-5 allegro_physfs-5 allegro_primitives-5 allegro_ttf-5
ALLEGRO_LIB_CFG=pkg-config --cflags --libs $(ALLEGRO_LIBS)

BUILD_DIR=build/
OUTPUT=ArchitectEngine

ENGINE_SOURCE_FILES= \
src/main.cpp \
src/com_main.cpp \
src/renderer/r_main.cpp \
src/renderer/r_image.cpp \
src/renderer/r_display.cpp \
src/renderer/r_material.cpp \
src/com_assetdb.cpp \
src/ui/ui_imgui.cpp \
src/ui/ui_atoms.cpp \
src/ui/ui_common.cpp \
src/ui/imgui/imgui.cpp \
src/ui/imgui/imgui_demo.cpp \
src/ui/imgui/imgui_draw.cpp 

engine: $(ENGINE_SOURCE_FILES)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LIBS) $(ENGINE_SOURCE_FILES) -o $(BUILD_DIR)$(OUTPUT) `$(ALLEGRO_LIB_CFG)`

game:
	echo "Not implemented yet"
