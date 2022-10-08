#pragma once

struct Vec2_t
{
	unsigned int x;
	unsigned int y;
};

typedef struct Vec2_t Vec2;

Vec2 NewVec2(int x, int y);