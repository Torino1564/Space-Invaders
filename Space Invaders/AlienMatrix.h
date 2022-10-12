#pragma once
#include "Entity.h"
#include "Vec2_F.h"

struct AlienMatrix_t
{
	Entity* matrix[4][11];

	Vec2F Pos;

	int width;
	int height;

	int AlienWidth;
	int AlienHeight;

	int AlienCount;
};

typedef struct AlienMatrix_t AlienMatrix;

AlienMatrix* NewMatrix( Vec2F pos_p, int width_p, int height_p, int Awidth_p, int Aheight_p);
void DestroyMatrix( AlienMatrix * matrix );