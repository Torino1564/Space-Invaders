#pragma once
#include "SpriteSheet.h"

SpriteSheet * NewSpriteSheet(char* texture, float maxDeltaFrame_p, int maxFrameCount_p, int frameWidth_p, int frameHeight_p, int maxAnimationNumber_p)
{
	SpriteSheet * TempSpriteSheet = malloc(sizeof(SpriteSheet));
	if (TempSpriteSheet == NULL)
	{
		return NULL;
	}

	TempSpriteSheet->Texture = al_load_bitmap(texture);
	if (TempSpriteSheet->Texture != NULL)
	{
		al_convert_mask_to_alpha(TempSpriteSheet->Texture, al_map_rgb(255, 0, 255));
	}
	
	TempSpriteSheet->maxDeltaFrame = maxDeltaFrame_p;
	TempSpriteSheet->maxFrameCount = maxFrameCount_p;
	TempSpriteSheet->frameHeight = frameHeight_p;
	TempSpriteSheet->frameWidth = frameWidth_p;
	TempSpriteSheet->maxAnimationNumber = maxAnimationNumber_p;
	TempSpriteSheet->animationNumber = 0;

	return TempSpriteSheet;
}

void DeleteSpriteSheet(SpriteSheet* Ssheet)
{
	if (Ssheet != NULL)
	{
		free(Ssheet);
	}

	return;
}