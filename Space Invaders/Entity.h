#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro5.h>
#include "Vec2.h"
#include "Graphics.h"
#ifndef RASPI
#include "Vec2_F.h"
#include "SpriteSheet.h"

typedef struct ALLEGRO_BITMAP ALLEGRO_BITMAP;

struct Entity_t
{
	Vec2F Pos;
	Vec2F Vel;

	ALLEGRO_BITMAP * Texture;
	SpriteSheet* spriteS;

	unsigned int width;
	unsigned int height;

	char animated;
	unsigned int frameCount;
	float deltaFrame;

	char data;

};
typedef struct Entity_t Entity;


void DrawEntity(Entity* Entity);

void UpdateEntity(Entity* Entity , double dt );
void ClipToScreen(Entity* Entity, Vec2 Screen);
void ClipToEntity(Entity* e1, Entity* e2, int adjust);

Entity* CreateNewEntity(Vec2F pos, Vec2F vel, const char * texture , int width , int height);
Entity* CreateNewEntityLoadedTexture(Vec2F pos, Vec2F vel, ALLEGRO_BITMAP* texture, int width, int height);
Entity* CreateNewAnimatedEntityLoadedTexture(Vec2F pos, Vec2F vel, SpriteSheet * SpriteSheet_p, int width_p, int height_p);

void DestroyEntity(Entity* Entity);
void DestroyEntityLoadedTexture(Entity* Entity);
void DestroyAnimatedEntity(Entity* Entity);
void DestroyAnimatedEntitySharedSprite(Entity* Entity);

int AreColiding(Entity* e1, Entity* e2);
int Animate(Entity* ent, float dt);

#endif

#ifdef RASPI
struct Entity_t
{
	Vec2 Pos;
	Vec2 Vel;

	double updateCooldown;
	double timeBuffer;

	char shape[25];
	Vec2 dimensions;

};

typedef struct Entity_t Entity;

Entity* CreateNewEntity(Vec2 pos, Vec2 vel, double updateCooldown, char * shape , Vec2 dimensions);
void DestroyEntity(Entity* entity);
void UpdateEntity(Entity* entity, double dt);
void DrawEntity(Entity* entity);
#endif


