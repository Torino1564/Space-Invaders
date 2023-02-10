#include "Graphics.h"

void ClearGrid()
{
#ifndef RASPI_TEST
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			Grid[i][j] = 0;
		}
	}
#endif
#ifdef RASPI_TEST
	disp_clear();
#endif
}

void TurnOn(int x, int y)
{
#ifndef RASPI_TEST
	if (x >= 0 && x < 16 && y >= 0 && y < 16)
		Grid[x][y] = true;
#endif
#ifdef RASPI_TEST
	dcoord_t TempPoint = { x , y };
	disp_write(TempPoint, D_ON);
#endif
}

void PrintGrid()
{
#ifndef RASPI_TEST
	int padding = 25;
	int startX = 50;
	int startY = 50;

	al_clear_to_color(al_map_rgb(0, 0, 30));

	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			if (Grid[i][j] == 1)
			{
				al_draw_filled_circle(startX + padding * i, startY + padding * j, 10, al_map_rgb(255, 0, 0));
			}
			else
			{
				al_draw_circle(startX + padding * i, startY + padding * j, 10, al_map_rgb(50, 50, 50), 1);

			}
		}

	}

	al_draw_rectangle(startX - padding, startY - padding, startX + padding * 16, startY + padding * 16, al_map_rgb(30, 30, 255), 5);
	al_flip_display();
#endif
#ifdef RASPI_TEST
	disp_update();
#endif

}

void InitGraphics()
{
#ifndef RASPI_TEST
	Display = al_create_display(800, 800);
#endif 
#ifdef RASPI_TEST
	disp_init();
#endif

}