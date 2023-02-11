#pragma once
#include "Entity.h"
#include "Vec2_F.h"
#include "Vec2.h"

#ifndef RASPI
#include "SpriteSheet.h"
#include "Animation.h"
struct AlienMatrix_t
{
	Entity * matrix[20][10];

	Vec2F Pos;

	int HorizontalSpeed;
	int VerticalSpeed;

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

	double MicroTimeBuffer;
};

typedef struct AlienMatrix_t AlienMatrix;

AlienMatrix* NewMatrix(int AlienPaddingX, int AlienPaddingY, int Awidth_p, int Aheight_p, int XAliens_p, int YAliens_p, int speed);
void DestroyMatrix( AlienMatrix * matrix );
int CollideGrid(Entity* Bullet[], AlienMatrix* Matrix, int aliensDestroyed, SpriteSheet* ExplosionSpritesheet);
int CollideEntity(Entity* BifUFOent, Entity* Bullet[]);
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
	Entity* matrix[6][4];

	Vec2 Pos;
	int HorizontalSpeed;
	int VerticalSpeed;

	int XAliens;
	int YAliens;

	Vec2 AlienDimensions;
	int AlienPadding;
	char Shape[25];

	int AlienCount;

	double MicroTimeBuffer;
};

typedef struct AlienMatrix_t AlienMatrix;

AlienMatrix* CreateNewAlienMatrix(Vec2 Pos, int HorizontalSpeed, int XAliens, int YAliens , Vec2 AlienDimensions , int Padding , char shape[]);
void FillMatrix(AlienMatrix* Matrix);
void UpdateMatrixDynamic(AlienMatrix* Matrix, double dt, Vec2 PlayAreaPos, Vec2 PlayAreaDim);
void DrawGrid(AlienMatrix* Matrix);
void DestroyAlienMatrix(AlienMatrix* Matrix);
int CollideGrid(Entity* Bullet[], AlienMatrix* Matrix, int* aliensDestroyed);




#endif

