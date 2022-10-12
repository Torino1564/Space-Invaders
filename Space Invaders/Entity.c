#pragma once
#include "Entity.h"

void DrawEntity(Entity* Entity)
{
	if (Entity->Texture != NULL)
	{
		al_draw_scaled_bitmap(Entity->Texture, 0, 0,
			al_get_bitmap_width(Entity->Texture), al_get_bitmap_height(Entity->Texture),
			Entity->Pos.x, Entity->Pos.y, Entity->width, Entity->height, NULL);
	}

	return;
}

void UpdateEntity(Entity* Entity , double dt)
{

	Entity->Pos.x += Entity->Vel.x * dt;
	Entity->Pos.y += Entity->Vel.y * dt;

	return;
}

void ClipToScreen(Entity* Entity, Vec2 Screen)
{

	if (Entity->Pos.x < 0)
	{
		Entity->Pos.x = 0;
	}
	else if (Entity->Pos.x + Entity->width > Screen.x)
	{
		Entity->Pos.x = Screen.x - Entity->width;
	}

	if (Entity->Pos.y < 0)
	{
		Entity->Pos.y = 0;
	}
	else if (Entity->Pos.y + Entity->height > Screen.y)
	{
		Entity->Pos.y = Screen.y - Entity->height;
	}
}

Entity* CreateNewEntity(Vec2F pos, Vec2F vel, const char* texture, int height, int width)
{
	Entity * ent = malloc(sizeof(Entity));
	if (ent == NULL)
	{
		return NULL;
	}
	ent->Pos = pos;
	ent->Vel = vel;
	ent->width = width;
	ent->height = height;

	ent->Texture = al_load_bitmap(texture);
	if (ent->Texture == NULL)
	{
		return NULL;
	}

	al_convert_mask_to_alpha(ent->Texture, al_map_rgb(255, 0, 255));


	return ent;
	
}

void DestroyEntity(Entity* Entity)
{
	if (Entity->Texture != NULL)
	al_destroy_bitmap(Entity->Texture);
	if (Entity != NULL)
	free(Entity);

	return;
}

int AreColiding(Entity* e1, Entity* e2)
{
	return (((e1->Pos.x > e2->Pos.x) && (e1->Pos.x < (e2->Pos.x + e2->width)))
		&& ((e1->Pos.y > e2->Pos.y) && (e1->Pos.y < (e2->Pos.y + e2->height))));
}
