#pragma once
#include "Shield.h"


shield* CreateNewShield(Vec2F pos, Vec2F vel, Vec2F Dimensions, uint16_t Xdivisions, char * Texture)
{
	shield* TempShield = malloc(sizeof(shield));
	if (TempShield == NULL)
	{
		return NULL;
	}
	
	TempShield->pos = pos;
	TempShield->vel = vel;
	TempShield->dimensions = Dimensions;

	TempShield->Xdivisions = Xdivisions;

	TempShield->animated = false;

	TempShield->Texture = al_load_bitmap(Texture);

	if (TempShield->Texture != NULL)
	{
		al_convert_mask_to_alpha(TempShield->Texture, al_map_rgb(255, 0, 255));
	}

	

	return TempShield;

}

void DrawShield(shield* shield)
{
	al_draw_scaled_bitmap(shield->Texture, 0, 0, al_get_bitmap_width(shield->Texture), al_get_bitmap_height(shield->Texture), shield->pos.x, shield->pos.y
		, shield->dimensions.x, shield->dimensions.y, NULL);
	return;
}

void FillShieldParticles(shield* shield)
{
	float Owidth = al_get_bitmap_width(shield->Texture);
	float Oheight = al_get_bitmap_height(shield->Texture);

	float OXDelta = Owidth / XDivisions;
	float OYDelta = Oheight / YDivisions;
	shield->Factors = NewVec2F(shield->dimensions.x / Owidth, shield->dimensions.y / Oheight);

	shield->particleSize.x = OXDelta * shield->Factors.x;
	shield->particleSize.y = OYDelta * shield->Factors.y;

	shield->Particles[0][0] = calloc(XDivisions * YDivisions, sizeof(uint8_t));
	for (int i = 0; i < XDivisions; i++)
	{
		for (int j = 0; j < YDivisions; j++)
		{
			shield->Particles[i][j] = false;
		}
	}

	uint8_t found = false;
	ALLEGRO_COLOR TempColor;
	for (int i = 0; i < XDivisions; i++)
	{
		for (int j = 0; j < YDivisions; j++)
		{
			TempColor = al_get_pixel(shield->Texture, i * OXDelta + OXDelta / 2, j * OYDelta + OYDelta / 2);
			if (TempColor.r != 0 || TempColor.g != 0 || TempColor.b != 0)
			{
				found = true;
			}

			if (found == true)
			{
				shield->Particles[i][j] = true;
				found = false;
			}
		}
	}

}

void DrawShieldPartitions(shield* shield)
{
	float Xdelta = (shield->particleSize.x / shield->Factors.x);
	float Ydelta = (shield->particleSize.y / shield->Factors.y);
	for (int i = 0; i < XDivisions; i++)
	{
		for (int j = 0 ; j < YDivisions; j++)
		{
			if (shield->Particles[i][j] == true)
			{
				/*al_draw_filled_rectangle(shield->pos.x + i * shield->particleSize.x, shield->pos.y + j * shield->particleSize.y,
					shield->pos.x + (i + 1) * shield->particleSize.x, shield->pos.y + (j + 1) * shield->particleSize.y , al_map_rgb(0,0,255));*/
				al_draw_scaled_bitmap(shield->Texture, i * Xdelta , j * Ydelta, Xdelta , Ydelta , shield->pos.x + i * shield->particleSize.x ,
					shield->pos.y + j * shield->particleSize.y , shield->particleSize.x , shield->particleSize.y , NULL);
			}
		}
	}

}