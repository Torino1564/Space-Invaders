#pragma once
#include "Entity.h"
#include "Vec2_F.h"

struct 

struct EntityMatrix_t
{
	Entity* (*matrix)[];

	Vec2F Pos;

	int HorizontalSpeed;

	int width;
	int height;

	int EntityWidth;
	int EntityHeight;

	Vec2 GridDimensions;
	int EntityPaddingX;
	int EntityPaddingY;

	int XEntity;
	int YEntity;

	int EntityCount;
};

typedef struct EntityMatrix_t EntityMatrix;

EntityMatrix* NewMatrix(Vec2F pos_p, int width_p, int height_p, int Ewidth_p, int Eheight_p, int speed, int XEntity, int YEntity);
void DestroyMatrix(EntityMatrix * matrix );
void CollideGrid(Entity* Bullet[], EntityMatrix* Matrix);
void DrawGrid(EntityMatrix* Matrix);
void SpawnMatrix(EntityMatrix* Matrix , ALLEGRO_BITMAP * texture);
void UpdateMatrix(EntityMatrix* Matrix, double dt , Vec2 ScreenDimensions);
