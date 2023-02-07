#pragma once

struct Vec2_t
{
	int x;
	int y;
};

typedef struct Vec2_t Vec2;

Vec2 NewVec2(int x, int y);
Vec2 SumVec2(Vec2* v1, Vec2* v2);
