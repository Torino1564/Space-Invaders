#pragma once
#include "Entity.h"

void DrawEntity(Entity* Entity)
{
	if (Entity != NULL)
	{
		if (Entity->animated == true)
		{
			if (Entity->spriteS->Texture != NULL)
			{
				al_draw_scaled_bitmap(Entity->spriteS->Texture, Entity->frameCount * Entity->spriteS->frameWidth, 0, Entity->spriteS->frameWidth, Entity->spriteS->frameHeight,
					Entity->Pos.x, Entity->Pos.y, Entity->width, Entity->height, NULL);
			}
			
		}
		else if(Entity->Texture != NULL)
		{
			al_draw_scaled_bitmap(Entity->Texture, 0, 0, al_get_bitmap_width(Entity->Texture), al_get_bitmap_height(Entity->Texture), Entity->Pos.x, Entity->Pos.y, Entity->width, Entity->height, NULL);
		}

		

	}
	return;
}

void UpdateEntity(Entity* Entity , double dt)
{
	if (Entity != NULL)
	{
		Entity->Pos.x += Entity->Vel.x * dt;
		Entity->Pos.y += Entity->Vel.y * dt;

	}

	

	return;
}

void ClipToScreen(Entity* Entity, Vec2 Screen)
{
	if (Entity != NULL)
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
	
}

void ClipToEntity(Entity* e1, Entity* e2, int adjust)
{
	if (e1 != NULL)
	{
		if (e1->Pos.x < e2->Pos.x + adjust)
		{
			e1->Pos.x = e2->Pos.x + adjust;
		}
		else if (e1->Pos.x > e2->Pos.x + adjust)
		{
			e1->Pos.x = e2->Pos.x + adjust;
		}
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

	ent->animated = false;

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

	ent->animated = false;

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
	if (Entity != NULL)
	{
		if (Entity->Texture != NULL)
		{
			al_destroy_bitmap(Entity->Texture);
			free(Entity);
		}
	}
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

Entity* CreateNewAnimatedEntityLoadedTexture(Vec2F pos, Vec2F vel, SpriteSheet* SpriteSheet_p, int width_p, int height_p)
{
	Entity* ent = malloc(sizeof(Entity));
	if (ent == NULL)
	{
		return ent;
	}
	ent->Pos = pos;
	ent->Vel = vel;

	if (SpriteSheet_p == NULL)
	{
		return NULL;
	}

	ent->spriteS = SpriteSheet_p;
	if (ent->spriteS->Texture != NULL)
	{
		al_convert_mask_to_alpha(ent->spriteS->Texture, al_map_rgb(255, 0, 255));
	}

	ent->animated = true;
	ent->frameCount = 0;

	ent->deltaFrame = 0;

	ent->width = width_p;
	ent->height = height_p;

	return ent;
}

void Animate(Entity* ent, float dt)
{
	ent->deltaFrame += dt;
	if (ent->deltaFrame >= ent->spriteS->maxDeltaFrame)
	{
		ent->deltaFrame = 0;
		ent->frameCount++;
		if (ent->frameCount >= ent->spriteS->maxFrameCount)
		{
			ent->frameCount = 0;
		}
	}
}
