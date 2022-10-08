#pragma once
#include "Entity.h"

void DrawEntity(Entity* Entity)
{
	if (Entity->Texture != NULL)
	al_draw_bitmap(Entity->Texture, Entity->Pos.x, Entity->Pos.y, NULL);
	return;
}

void UpdateEntity(Entity* Entity)
{
	Entity->Pos.x += Entity->Vel.x;
	Entity->Pos.y += Entity->Vel.y;

	return;
}
