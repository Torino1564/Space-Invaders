#pragma once
#include "Entity.h"

void DrawEntity(Entity* Entity)
{
	if (Entity != NULL)
	{
		if (Entity->Texture != NULL)
		{
			al_draw_scaled_bitmap(Entity->Texture, 0, 0,
				al_get_bitmap_width(Entity->Texture), al_get_bitmap_height(Entity->Texture),
				Entity->Pos.x, Entity->Pos.y, Entity->width, Entity->height, NULL);
		}
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

Entity* CreateNewEntityLoadedTexture(Vec2F pos, Vec2F vel, ALLEGRO_BITMAP* texture, int width, int height)
{
	Entity* ent = malloc(sizeof(Entity));
	if (ent == NULL)
	{
		return NULL;
	}
	ent->Pos = pos;
	ent->Vel = vel;
	ent->width = width;
	ent->height = height;

	if (texture != NULL)
	{
		ent->Texture = texture;
	}
	else
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

void DestroyEntityLoadedTexture(Entity* Entity)
{
	if (Entity != NULL)
	{
		free(Entity);
	}
	return;
}

int AreColiding(Entity* e0, Entity* e1)
{
	if (e1 != NULL && e0 != NULL)
	{
		float right0 = e0->Pos.x + e0->width;
		float bottom0 = e0->Pos.y + e0->height;
		float right1 = e1->Pos.x + e1->width;
		float bottom1 = e1->Pos.y + e1->height;

		return
			right0 >= e1->Pos.x &&
			e0->Pos.x <= right1 &&
			bottom0 >= e1->Pos.y &&
			e0->Pos.y <= bottom1;
	}
	else return 0;
}
