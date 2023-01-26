#include "Math.h"

float AbsoluteValue(float x)
{
	if (x >= 0)
	{
		return x;
	}
	else
	{
		return -x;
	}
}

float MaxClampTo(float x, float max)
{
	if (x >= max) return max;
	else return x;
}

float MinClampTo(float x, float min)
{
	if (x <= min) return min;
	else return x;
}

int IMaxClampTo(int x, int max)
{
	if (x >= max) return max;
	else return x;
}

int IMinClampTo(int x, int min)
{
	if (x <= min) return min;
	else return x;
}