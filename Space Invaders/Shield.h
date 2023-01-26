#pragma once
#include "Vec2.h"
#include "Vec2_F.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro5.h>
#include "SpriteSheet.h"
#include <allegro5/allegro_primitives.h>

#define XDivisions 30
#define YDivisions 20

struct shield_t
{
	Vec2F pos;
	Vec2F vel;

	Vec2F dimensions;
	uint16_t Xdivisions;
	uint16_t Ydivisions;

	Vec2F particleSize;
	Vec2F Factors;

	float OXdelta;

	ALLEGRO_BITMAP* Texture;
	SpriteSheet* Sprite;

	uint8_t animated;

	char destroyed;
	int originalSize;

	uint8_t * Particles[XDivisions][YDivisions];
};

typedef struct shield_t shield;

shield* CreateNewShield(Vec2F pos, Vec2F vel, Vec2F Dimensions, uint16_t Xdivisions, char * Texture);
void DrawShield(shield* shield);
void FillShieldParticles(shield* shield);
void DrawShieldPartitions(shield* shield);