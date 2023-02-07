#include "Graphics.h"

void ClearGrid()
{
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			Grid[i][j] = 0;
		}
	}
}

void TurnOn(int x, int y)
{
	if (x >= 0 && x < 16 && y >= 0 && y < 16)
		Grid[x][y] = true;
}

void PrintGrid()
{
	
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
				al_draw_circle(startX + padding * i, startY + padding * j, 10, al_map_rgb(200, 200, 200), 1);

			}
		}

	}

	al_draw_rectangle(startX - padding, startY - padding, startX + padding * 16, startY + padding * 16, al_map_rgb(30, 30, 255), 5);
	al_flip_display();
}