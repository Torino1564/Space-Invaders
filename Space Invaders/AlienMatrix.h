#pragma once
#include "Entity.h"
#include "Vec2_F.h"

struct AlienMatrix_t
{
	Entity * (* matrix)[];

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

AlienMatrix* NewMatrix(Vec2F pos_p, int width_p, int height_p, int Awidth_p, int Aheight_p, int speed, int Xaliens, int Yaliens);
void DestroyMatrix( AlienMatrix * matrix );
void CollideGrid(Entity* Bullet[], AlienMatrix* Matrix);
void DrawGrid(AlienMatrix* Matrix);
void SpawnMatrix(AlienMatrix* Matrix , ALLEGRO_BITMAP * texture);
void UpdateMatrix(AlienMatrix* Matrix, double dt , Vec2 ScreenDimensions);
