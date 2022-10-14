#pragma once
#include "AlienMatrix.h"

AlienMatrix* NewMatrix( Vec2F pos_p, int width_p, int height_p, int Awidth_p, int Aheight_p , int speed)
{
	AlienMatrix* TempMatrix = malloc(sizeof(AlienMatrix));
	if (TempMatrix != NULL)
	{
		TempMatrix->Pos = pos_p;
		TempMatrix->width = width_p;
		TempMatrix->height = height_p;
		TempMatrix->AlienHeight = Aheight_p;
		TempMatrix->AlienWidth = Awidth_p;

		TempMatrix->AlienPaddingX = (TempMatrix->width - TempMatrix->AlienWidth * 11) / 10;
		TempMatrix->AlienPaddingY = (TempMatrix->height - TempMatrix->AlienHeight * 4) / 3;

		TempMatrix->HorizontalSpeed = speed;

	}

	return TempMatrix;
}

void DestroyMatrix(AlienMatrix* matrix)
{
	free(matrix);
}

void DrawGrid(AlienMatrix* Matrix)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			if (Matrix->matrix[i][j] != NULL)
			{
				DrawEntity(Matrix->matrix[i][j]);
			}
		}
	}
}

void SpawnMatrix(AlienMatrix* Matrix , ALLEGRO_BITMAP * texture)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			if (Matrix->matrix[i][j] == NULL)
			{
				Matrix->matrix[i][j] = CreateNewEntityLoadedTexture(
					NewVec2F(Matrix->Pos.x + (50 + Matrix->AlienPaddingX) * j, Matrix->Pos.y + (50 + Matrix->AlienPaddingY) * i),
					NewVec2F(0, 0), texture, 50, 50);
			}
			DrawEntity(Matrix->matrix[i][j]);
			al_flip_display();
			al_rest(0.02);
		}
	}

	Matrix->AlienCount = 4 * 11;
}

void CollideGrid(Entity* Bullet[], AlienMatrix* Matrix)
{
	for (int b = 0; b < 10; b++)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 11; j++)
			{
				if (Bullet[b] != NULL && Matrix->matrix[i][j] != NULL)
				{
					if (AreColiding(Matrix->matrix[i][j] , Bullet[b]))
					{
						DestroyEntityLoadedTexture(Bullet[b]);
						Bullet[b] = NULL;

						DestroyEntityLoadedTexture(Matrix->matrix[i][j]);
						Matrix->matrix[i][j] = NULL;

						Matrix->AlienCount -= 1;
					}
				}
			}
		}
	}
}

void UpdateMatrix(AlienMatrix* Matrix, double dt , Vec2 ScreenDimensions)
{
	static double timeBuffer;
	timeBuffer = timeBuffer + dt;

	if (timeBuffer >= 1)
	{
		timeBuffer = 0;
		Matrix->Pos.x += Matrix->HorizontalSpeed;
		
		if ((Matrix->Pos.x + Matrix->width >= ScreenDimensions.x) || (Matrix->Pos.x - 2 * Matrix->AlienWidth < 0 ))
		{
			Matrix->HorizontalSpeed *= -1;
		}

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 11; j++)
			{
				if (Matrix->matrix[i][j] != NULL)
				Matrix->matrix[i][j]->Pos.x += Matrix->HorizontalSpeed;
			}
		}
	}

	return;
}