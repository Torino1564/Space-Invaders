#pragma once
#include "Vec2.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro5.h>

typedef struct ALLEGRO_BITMAP ALLEGRO_BITMAP;

struct Entity_t
{
	Vec2 Pos;

	ALLEGRO_BITMAP * Texture;

	unsigned int width;
	unsigned int height;

};
typedef struct Entity_t Entity;

void DrawEntity(Entity* Entity);