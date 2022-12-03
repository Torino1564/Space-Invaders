#pragma once
#include "Vec2.h"
#include "Vec2_F.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro5.h>

struct SpriteSheet_t
{
	ALLEGRO_BITMAP* Texture;
	float maxDeltaFrame;
	int maxFrameCount;
	int frameWidth;
	int frameHeight;
	int animationNumber;
	int maxAnimationNumber;

};

typedef struct SpriteSheet_t SpriteSheet;

SpriteSheet* NewSpriteSheet(char* texture, float maxDeltaFrame_p, int maxFrameCount_p, int frameWidth_p, int frameHeight_p, int maxAnimationNumber_p);
