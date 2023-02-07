#pragma once
#include "Vec2.h"

Vec2 NewVec2(int x, int y)
{
	Vec2 Vector;
	Vector.x = x;
	Vector.y = y;

	return Vector;
}

Vec2 SumVec2(Vec2* v1, Vec2* v2)
{
	return NewVec2( v1->x + v2->x , v1->y + v2->y );
}
