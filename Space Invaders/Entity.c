#pragma once
#include "Entity.h"
#ifndef RASPI

void DrawEntity(Entity* Entity)
{
	if (Entity != NULL)
	{
		if (Entity->data != 66)
		{
			if (Entity->animated == true)
			{
				if (Entity->spriteS->Texture != NULL && Entity->spriteS->animationNumber < Entity->spriteS->maxAnimationNumber)
				{
					al_draw_scaled_bitmap(Entity->spriteS->Texture, Entity->frameCount * Entity->spriteS->frameWidth, Entity->spriteS->frameHeight * Entity->spriteS->animationNumber, Entity->spriteS->frameWidth, Entity->spriteS->frameHeight,
						Entity->Pos.x, Entity->Pos.y, Entity->width, Entity->height, 0);
				}

			}
			else if (Entity->Texture != NULL)
			{
				al_draw_scaled_bitmap(Entity->Texture, 0, 0,
					al_get_bitmap_width(Entity->Texture), al_get_bitmap_height(Entity->Texture),
					Entity->Pos.x, Entity->Pos.y, Entity->width, Entity->height, 0);
			}
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

void ClipToScreen(Entity* Entity, Vec2F Screen)
{
	if (Entity != NULL)
	{
		if (Entity->Pos.x < 2*Entity->width)
		{
			Entity->Pos.x = 2*Entity->width;
		}
		else if (Entity->Pos.x + Entity->width > (Screen.x - 2*Entity->width))
		{
			Entity->Pos.x = Screen.x - 3*Entity->width;
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
	ent->data = 0;


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

	ent->data = 0;

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

void DestroyAnimatedEntity(Entity* Entity)
{
	if (Entity != NULL)
	{
		if (Entity->spriteS != NULL)
		{
			DeleteSpriteSheet(Entity->spriteS);
			free(Entity);
		}
	}
	return;
}

void DestroyAnimatedEntitySharedSprite(Entity* Entity)
{
	if (Entity != NULL)
	{
		if (Entity->spriteS != NULL)
		{
			free(Entity);
		}
	}
	return;
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

	ent->data = 0;

	return ent;
}

int Animate(Entity* ent, float dt) //devuelve 1 cuando da una vuelta por todo el spritesheet
{
	if (ent != NULL)
	{
		ent->deltaFrame += dt;
		if (ent->deltaFrame >= ent->spriteS->maxDeltaFrame)
		{
			ent->deltaFrame -= ent->spriteS->maxDeltaFrame;
			ent->frameCount++;
			if (ent->frameCount >= ent->spriteS->maxFrameCount)
			{
				ent->frameCount = 0;
				return 1;
			}
		}
	}
	return 0;
}
#endif

#ifdef RASPI
Entity* CreateNewEntity(Vec2 pos, Vec2 vel, double updateCooldown, char shape[], Vec2 dimensions)
{
	Entity* TempEntity = malloc(sizeof(Entity));
	if (TempEntity == NULL) return NULL;

	TempEntity->Pos = pos;
	TempEntity->Vel = vel;
	TempEntity->updateCooldown = updateCooldown;
	TempEntity->dimensions = dimensions;
	TempEntity->timeBuffer = 0;

	for (int i = 0; i < MAX_SHAPE; i++)
	{
		if (i < (dimensions.x * dimensions.y))
		{
			TempEntity->shape[i] = shape[i];
		}
		else TempEntity->shape[i] = 0;
	}

	return TempEntity;

}

void DestroyEntity(Entity* entity)
{
	if (entity != NULL)
	{
		free(entity);
	}
}

void UpdateEntity(Entity* entity, double dt)
{
	if (entity != NULL)
	{
		entity->timeBuffer += dt;
		if (entity->timeBuffer >= entity->updateCooldown)
		{
			entity->timeBuffer -= entity->updateCooldown;
			entity->Pos = SumVec2(&entity->Pos, &entity->Vel);
		}
	}
}

void DrawEntity(Entity* entity)
{
	if (entity != NULL)
	{
		for (int i = 0; i < entity->dimensions.x; i++)
		{
			for (int j = 0; j < entity->dimensions.y; j++)
			{
				if (entity->shape[i + entity->dimensions.x * j])
				{
					if (entity->Pos.x + i < 16 && entity->Pos.y + j < 16)
					{
						TurnOn(entity->Pos.x + i, entity->Pos.y + j);
					}
				}
			}
		}
	}
}

void ClamToScreen(Entity* entity)
{
	if (entity->Pos.x < 0)
	{
		entity->Pos.x = 0;
	}
	if (entity->Pos.x + entity->dimensions.x >= 16)
	{
		entity->Pos.x = 16 - entity->dimensions.x;
	}
}

int ColideAndDestroy(Entity* destroyed, Entity* e1)
{
	if (e1 != NULL && destroyed != NULL)
	{
		float right0 = destroyed->Pos.x + destroyed->dimensions.x - 1;
		float bottom0 = destroyed->Pos.y + destroyed->dimensions.y - 1;
		float right1 = e1->Pos.x + e1->dimensions.x - 1;
		float bottom1 = e1->Pos.y + e1->dimensions.y - 1;

		if (right0 >= e1->Pos.x &&
			destroyed->Pos.x <= right1 &&
			bottom0 >= e1->Pos.y &&
			destroyed->Pos.y <= bottom1)
		{
			int position = e1->Pos.x - destroyed->Pos.x + (destroyed->dimensions.x * (e1->Pos.y - destroyed->Pos.y));
			if (position < destroyed->dimensions.x * destroyed->dimensions.y)
			{
				if (destroyed->shape[position] == true)
				{
					destroyed->shape[position] = 0;
					return true;
				}
				else
				{
					return false;
				}
				
			}

		}
	}
	return false;
}

#endif

int AreColiding(Entity* e0, Entity* e1)
{
	if (e1 != NULL && e0 != NULL)
	{
#ifndef RASPI
		float right0 = e0->Pos.x + e0->width;
		float bottom0 = e0->Pos.y + e0->height;
		float right1 = e1->Pos.x + e1->width;
		float bottom1 = e1->Pos.y + e1->height;
#endif
#ifdef RASPI
		float right0 = e0->Pos.x + e0->dimensions.x - 1;
		float bottom0 = e0->Pos.y + e0->dimensions.y - 1;
		float right1 = e1->Pos.x + e1->dimensions.x - 1;
		float bottom1 = e1->Pos.y + e1->dimensions.y - 1;
#endif

		return
			right0 >= e1->Pos.x &&
			e0->Pos.x <= right1 &&
			bottom0 >= e1->Pos.y &&
			e0->Pos.y <= bottom1;
	}
	else return 0;
}

