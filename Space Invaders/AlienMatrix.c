#pragma once
#include "AlienMatrix.h"

AlienMatrix* NewMatrix( Vec2F pos_p, int width_p, int height_p, int Awidth_p, int Aheight_p)
{
	AlienMatrix* TempMatrix = malloc(sizeof(AlienMatrix));
	if (TempMatrix != NULL)
	{
		TempMatrix->Pos = pos_p;
		TempMatrix->width = width_p;
		TempMatrix->height = height_p;
		TempMatrix->AlienHeight = Aheight_p;
		TempMatrix->AlienWidth = Awidth_p;

	}

	return TempMatrix;
}

void DestroyMatrix(AlienMatrix* matrix)
{
	free(matrix);
}