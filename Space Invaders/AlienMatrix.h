#pragma once
#include "Entity.h"
#include "Vec2_F.h"
#include "Vec2.h"
#include "SpriteSheet.h"
#include "Animation.h"
#ifndef RASPI

struct AlienMatrix_t
{
	Entity * matrix[20][10];

	Vec2F Pos;

	int HorizontalSpeed;

	int width;
	int height;

	int AlienWidth;
	int AlienHeight;

	Vec2 GridDimensions;
	int AlienPaddingX;
	int AlienPaddingY;

	int XAliens;
	int YAliens;

	int AlienCount;
};

typedef struct AlienMatrix_t AlienMatrix;

AlienMatrix* NewMatrix(int AlienPaddingX, int AlienPaddingY, int Awidth_p, int Aheight_p, int XAliens_p, int YAliens_p, int speed);
void DestroyMatrix( AlienMatrix * matrix );
int CollideGrid(Entity* Bullet[], AlienMatrix* Matrix, int aliensDestroyed, SpriteSheet* ExplosionSpritesheet);
void DrawGrid(AlienMatrix* Matrix);
void FillMatrix(AlienMatrix* Matrix , ALLEGRO_BITMAP * texture);
void FillMatrixAnimated(AlienMatrix* Matrix, SpriteSheet* SpriteSheet_p);
void UpdateMatrix(AlienMatrix* Matrix, double dt, Vec2 PlayAreaPos, Vec2 PlayAreaDim);
void UpdateMatrixDynamic(AlienMatrix* Matrix, double dt, Vec2 PlayAreaPos, Vec2 PlayAreaDim);
Vec2F GetCentredPosition(AlienMatrix* Matrix, Vec2 ScreenDimension);
void AnimateMatrix(AlienMatrix* Matrix, float dt);
void DeathAnimation(int i, int j);
#endif

#ifdef RASPI

struct AlienMatrix_t
{
	Vec2 pos;
};

typedef struct AlienMatrix_t AlienMatrix;

#endif