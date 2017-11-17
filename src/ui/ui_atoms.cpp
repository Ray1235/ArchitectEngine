#include "../common.h"

void UI_DrawPicEx(int sourceX, int sourceY, int sourceW, int sourceH, int destinationX, int destinationY, int flags)
{
	al_draw_bitmap_region(NULL, sourceX, sourceY, sourceW, sourceH, destinationX, destinationY, flags);
}