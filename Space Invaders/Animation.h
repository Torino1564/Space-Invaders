#pragma once
#include "Entity.h"
#include "SpriteSheet.h"
#include "Vec2_F.h"

#define MAX_ANIMATION_BUFFER_SIZE 30

struct Animation_t
{
	Entity* Entity;
	uint8_t Loops;
	uint16_t BufferIndex;

};

typedef struct Animation_t Animation;

Animation * (*AnimationBuffer)[];

int InitAnimations();
int CreateNewAnimation(Vec2F pos, Vec2F vel, uint8_t Loops, SpriteSheet * Sprite);