#include "Animation.h"
#pragma once

int InitAnimations()
{
	AnimationBuffer = calloc(MAX_ANIMATION_BUFFER_SIZE, sizeof( Animation *));

	return 0;
}

int CreateNewAnimation(Vec2F pos, Vec2F vel, uint8_t Loops, SpriteSheet * Sprite)
{
	Animation * TempAnimation = malloc(sizeof(Animation));
	if (TempAnimation == NULL)
	{
		return 0;
	}

	Entity* TempEntity = CreateNewAnimatedEntityLoadedTexture(pos, vel, Sprite, 0, 0);
	if (TempEntity == NULL)
	{
		return 0;
	}

	TempAnimation->Entity = TempEntity;
	TempAnimation->Loops = Loops;

	int i;
	for (i = 0; i < MAX_ANIMATION_BUFFER_SIZE; i++)
	{
		if ((*AnimationBuffer)[i] != NULL)
		{
			(*AnimationBuffer)[i] = TempAnimation;
			TempAnimation->BufferIndex = i;
			break;
		}
	}
	if (i == MAX_ANIMATION_BUFFER_SIZE)
	{
		(*AnimationBuffer)[i] = TempAnimation;
		TempAnimation->BufferIndex = i;
	}

	return 1;
}

/*
int DeleteAnimation(Animation* Animation)
{
	if (Animation != NULL)
	{
		if ( )
	}
}
*/