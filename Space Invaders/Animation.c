#include "Animation.h"
#pragma once
#ifndef RASPI

int InitAnimations()
{
	AnimationBuffer = calloc(MAX_ANIMATION_BUFFER_SIZE, sizeof( Animation *));

	if (AnimationBuffer == NULL)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int CreateNewAnimation(Vec2F pos, Vec2F vel, uint8_t Loops, SpriteSheet * Sprite , float width , float height)
{
	Animation * TempAnimation = malloc(sizeof(Animation));
	if (TempAnimation == NULL)
	{
		return 0;
	}

	Entity* TempEntity = CreateNewAnimatedEntityLoadedTexture(pos, vel, Sprite, width, height);
	if (TempEntity == NULL)
	{
		return 0;
	}

	TempAnimation->Entity = TempEntity;
	TempAnimation->Loops = Loops;

	for (int i = 1; i < MAX_ANIMATION_BUFFER_SIZE; i++)
	{
		if ((*AnimationBuffer)[i] == NULL)
		{
			(*AnimationBuffer)[i] = TempAnimation;
			TempAnimation->BufferIndex = i;
			return i;
		}
	}
	return 1;
}

void UpdateAnimations( double dt)
{
	if (AnimationBuffer == NULL)
		return;

	for (int i = 1; i < MAX_ANIMATION_BUFFER_SIZE; i++)
	{
		if ((*AnimationBuffer)[i] != NULL)
		{
			UpdateEntity((*AnimationBuffer)[i]->Entity, dt);
			if (Animate((*AnimationBuffer)[i]->Entity, dt) == 1)
			{
				(*AnimationBuffer)[i]->Loops--;

				if ((*AnimationBuffer)[i]->Loops < 0)
				{
					DestroyAnimatedEntitySharedSprite((*AnimationBuffer)[i]->Entity);
					free((*AnimationBuffer)[i]);
					(*AnimationBuffer)[i] = NULL;


				}
			}

		}
	}


}

void DrawAnimations()
{
	for (int i = 1; i < MAX_ANIMATION_BUFFER_SIZE; i++)
	{
		if ((*AnimationBuffer)[i] != NULL)
		{
			DrawEntity((*AnimationBuffer)[i]->Entity);
		}
	}
}

void DestroyAnimation(Animation* animation)
{
	DestroyAnimatedEntitySharedSprite(animation->Entity);

	int tempBufferIndex = animation->BufferIndex;

	free((*AnimationBuffer)[animation->BufferIndex]);
	(*AnimationBuffer)[tempBufferIndex] = NULL;

}
#endif
