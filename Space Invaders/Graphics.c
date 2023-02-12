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

	dcoord_t TempPoint;

	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			TempPoint = (dcoord_t){ i,j };
			disp_write(TempPoint, D_OFF);

		}
	}
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
				//al_draw_circle(startX + padding * i, startY + padding * j, 10, al_map_rgb(50, 50, 50), 1);

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

void PrintNumber(int number, Vec2 pos)
{
    switch (number)
    {
    case 0:
        TurnOn(pos.x, pos.y);
        TurnOn(pos.x + 1, pos.y);
        TurnOn(pos.x + 2, pos.y);
        TurnOn(pos.x + 3, pos.y);

        TurnOn(pos.x + 3, pos.y + 1);
        TurnOn(pos.x + 3, pos.y + 2);
        TurnOn(pos.x + 3, pos.y + 3);
        TurnOn(pos.x + 3, pos.y + 4);

        TurnOn(pos.x + 2, pos.y + 4);
        TurnOn(pos.x + 1, pos.y + 4);
        TurnOn(pos.x, pos.y + 4);

        TurnOn(pos.x, pos.y + 3);
        TurnOn(pos.x, pos.y + 2);
        TurnOn(pos.x, pos.y + 1);
        break;
    case 1:
        TurnOn(pos.x + 3, pos.y);
        TurnOn(pos.x + 3, pos.y + 1);
        TurnOn(pos.x + 3, pos.y + 2);
        TurnOn(pos.x + 3, pos.y + 3);
        TurnOn(pos.x + 3, pos.y + 4);
        break;
    case 2:
        TurnOn(pos.x, pos.y+1);
        TurnOn(pos.x + 1, pos.y);
        TurnOn(pos.x + 2, pos.y);
        TurnOn(pos.x + 3, pos.y + 1);

        TurnOn(pos.x + 2, pos.y + 2);
        TurnOn(pos.x + 1, pos.y + 3);
        TurnOn(pos.x + 3, pos.y + 4);

        TurnOn(pos.x + 2, pos.y + 4);
        TurnOn(pos.x + 1, pos.y + 4);
        TurnOn(pos.x, pos.y + 4);

        TurnOn(pos.x, pos.y + 1);
        break;
    case 3:
        TurnOn(pos.x, pos.y);
        TurnOn(pos.x + 1, pos.y);
        TurnOn(pos.x + 2, pos.y);
        TurnOn(pos.x + 3, pos.y);

        TurnOn(pos.x + 3, pos.y + 2);
        TurnOn(pos.x + 2, pos.y + 2);
        TurnOn(pos.x + 1, pos.y + 2);
        TurnOn(pos.x , pos.y + 2);

        TurnOn(pos.x + 3, pos.y + 3);
        TurnOn(pos.x + 3, pos.y + 4);
        TurnOn(pos.x + 2, pos.y + 4);
        TurnOn(pos.x + 1, pos.y + 4);
        TurnOn(pos.x, pos.y + 4);

        TurnOn(pos.x + 2, pos.y);
        TurnOn(pos.x + 1, pos.y);
        TurnOn(pos.x, pos.y);

        TurnOn(pos.x + 3, pos.y + 1);
        TurnOn(pos.x + 3, pos.y + 2);
        break;
    case 4:
        TurnOn(pos.x, pos.y);
        TurnOn(pos.x, pos.y + 1);
        TurnOn(pos.x, pos.y + 2);
        TurnOn(pos.x + 1, pos.y + 2);
        TurnOn(pos.x + 2, pos.y + 2);
        TurnOn(pos.x + 3, pos.y + 2);
        TurnOn(pos.x + 3, pos.y + 1);
        TurnOn(pos.x + 3, pos.y);

        TurnOn(pos.x + 3, pos.y);
        TurnOn(pos.x + 3, pos.y + 3);
        TurnOn(pos.x + 3, pos.y + 4);

        break;
    case 5:
        TurnOn(pos.x, pos.y);
        TurnOn(pos.x + 1, pos.y);
        TurnOn(pos.x + 2, pos.y);
        TurnOn(pos.x + 3, pos.y);
        TurnOn(pos.x + 1, pos.y + 2);
        TurnOn(pos.x + 2, pos.y + 2);


        TurnOn(pos.x + 3, pos.y + 2);
        TurnOn(pos.x + 3, pos.y + 3);
        TurnOn(pos.x + 3, pos.y + 4);

        TurnOn(pos.x + 2, pos.y + 4);
        TurnOn(pos.x + 1, pos.y + 4);
        TurnOn(pos.x, pos.y + 4);

        TurnOn(pos.x, pos.y + 2);
        TurnOn(pos.x, pos.y + 1);
        break;
    case 6:
        TurnOn(pos.x + 3, pos.y);
        TurnOn(pos.x + 2, pos.y);
        TurnOn(pos.x + 1, pos.y);
        TurnOn(pos.x, pos.y);

        TurnOn(pos.x, pos.y + 1);
        TurnOn(pos.x, pos.y + 2);
        TurnOn(pos.x, pos.y + 3);
        TurnOn(pos.x, pos.y + 4);

        TurnOn(pos.x + 1, pos.y + 2);
        TurnOn(pos.x + 2, pos.y + 2);
        TurnOn(pos.x + 3, pos.y + 2);
        TurnOn(pos.x + 3, pos.y + 3);
        TurnOn(pos.x + 3, pos.y + 4);
        TurnOn(pos.x + 2, pos.y + 4);
        TurnOn(pos.x + 1, pos.y + 4);
        break;
    case 7:
        TurnOn(pos.x, pos.y);
        TurnOn(pos.x + 1, pos.y);
        TurnOn(pos.x + 2, pos.y);
        TurnOn(pos.x + 3, pos.y);
        TurnOn(pos.x + 3, pos.y + 1);
        TurnOn(pos.x + 3, pos.y + 2);
        TurnOn(pos.x + 3, pos.y + 3);


        TurnOn(pos.x + 3, pos.y + 4);
        break;
    case 8:
        TurnOn(pos.x, pos.y);
        TurnOn(pos.x + 1, pos.y);
        TurnOn(pos.x + 2, pos.y);
        TurnOn(pos.x + 3, pos.y);

        TurnOn(pos.x + 3, pos.y + 1);
        TurnOn(pos.x + 3, pos.y + 2);
        TurnOn(pos.x + 2, pos.y + 2);
        TurnOn(pos.x + 1, pos.y + 2);

        TurnOn(pos.x + 3, pos.y + 3);
        TurnOn(pos.x + 3, pos.y + 4);

        TurnOn(pos.x + 2, pos.y + 4);
        TurnOn(pos.x + 1, pos.y + 4);
        TurnOn(pos.x, pos.y + 4);

        TurnOn(pos.x, pos.y + 1);
        TurnOn(pos.x, pos.y + 2);
        TurnOn(pos.x, pos.y + 3);
        break;
    case 9:
        TurnOn(pos.x, pos.y);
        TurnOn(pos.x + 1, pos.y);
        TurnOn(pos.x + 2, pos.y);
        TurnOn(pos.x + 3, pos.y);

        TurnOn(pos.x + 3, pos.y + 1);
        TurnOn(pos.x + 3, pos.y + 2);
        TurnOn(pos.x + 3, pos.y + 3);
        TurnOn(pos.x + 3, pos.y + 4);

        TurnOn(pos.x + 2, pos.y + 2);
        TurnOn(pos.x + 1, pos.y + 2);

        TurnOn(pos.x, pos.y + 2);
        TurnOn(pos.x, pos.y + 1);
        break;
	}
}