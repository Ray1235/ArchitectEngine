CC=g++
CFLAGS=-Wall

ALLEGRO_LIBS=allegro-5 allegro_acodec-5 allegro_audio-5 allegro_color-5 allegro_dialog-5 allegro_font-5 allegro_image-5 allegro_main-5 allegro_memfile-5 allegro_physfs-5 allegro_primitives-5 allegro_ttf-5
LIBS=pkg-config --cflags --libs $(ALLEGRO_LIBS)
OUTPUT=ArchitectEngine

SOURCE_FILES=src/main.cpp src/com_main.cpp src/renderer/r_main.cpp src/renderer/r_display.cpp src/ui/ui_common.cpp src/ui/ui_imgui.cpp src/ui/imgui/imgui.cpp src/ui/imgui/imgui_demo.cpp src/ui/imgui/imgui_draw.cpp

Game: $(SOURCE_FILES)
	$(CC) $(CFLAGS) $(SOURCE_FILES) -o build/$(OUTPUT) `$(LIBS)`