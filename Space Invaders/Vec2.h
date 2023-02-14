#pragma once
#include <math.h>
struct Vec2_t
{
	int x;
	int y;
};

typedef struct Vec2_t Vec2;

Vec2 NewVec2(int x, int y);
Vec2 SumVec2(Vec2* v1, Vec2* v2);
Vec2 SubVec2(Vec2* v1, Vec2* v2);

float SquaredModulus(Vec2 vector);
float Modulus(Vec2 Vector);
