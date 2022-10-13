#pragma once
#include "Vec2.h"
#include "Vec2_F.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro5.h>

typedef struct ALLEGRO_BITMAP ALLEGRO_BITMAP;

struct Entity_t
{
	Vec2F Pos;
	Vec2F Vel;

	ALLEGRO_BITMAP * Texture;

	unsigned int width;
	unsigned int height;

};
typedef struct Entity_t Entity;

void DrawEntity(Entity* Entity);

void UpdateEntity(Entity* Entity , double dt );
void ClipToScreen(Entity* Entity, Vec2 Screen);
Entity* CreateNewEntity(Vec2F pos, Vec2F vel, const char * texture , int width , int height);
Entity* CreateNewEntityLoadedTexture(Vec2F pos, Vec2F vel, ALLEGRO_BITMAP* texture, int width, int height);

void DestroyEntity(Entity* Entity);
void DestroyEntityLoadedTexture(Entity* Entity);
int AreColiding(Entity* e1, Entity* e2);