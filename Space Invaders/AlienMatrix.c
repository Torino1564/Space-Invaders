#pragma once
#include "AlienMatrix.h"

AlienMatrix* NewMatrix( int AlienPaddingX, int AlienPaddingY , int Awidth_p , int Aheight_p, int XAliens_p, int YAliens_p, int speed)
{
	AlienMatrix* TempMatrix = malloc(sizeof(AlienMatrix));
	if (TempMatrix != NULL)
	{
		TempMatrix->AlienHeight = Aheight_p;
		TempMatrix->AlienWidth = Awidth_p;
		TempMatrix->XAliens = XAliens_p;
		TempMatrix->YAliens = YAliens_p;
		TempMatrix->AlienCount = 0;

		TempMatrix->AlienPaddingX = AlienPaddingX;
		TempMatrix->AlienPaddingY = AlienPaddingY;

		TempMatrix->width = Awidth_p * XAliens_p + (XAliens_p - 1) * AlienPaddingX;
		TempMatrix->height = Aheight_p * YAliens_p + (YAliens_p - 1) * AlienPaddingY;

		TempMatrix->HorizontalSpeed = speed;

		for (int i = 0; i < TempMatrix->XAliens; i++)
		{
			for (int j = 0; j < TempMatrix->YAliens; j++)
			{
				TempMatrix->matrix[i][j] = NULL;
			}
		}

	}

	return TempMatrix;
}

void DestroyMatrix(AlienMatrix* matrix)
{
	for (int i = 0; i < matrix->XAliens; i++)
	{
		for (int j = 0; j < matrix->YAliens; j++)
		{
			if (matrix->matrix[i][j] != NULL)
			{
				DestroyEntityLoadedTexture(matrix->matrix[i][j]);
			}
		}
	}
	free(matrix);
}

void DrawGrid(AlienMatrix* Matrix)
{
	for (int i = 0; i < Matrix->XAliens; i++)
	{
		for (int j = 0; j < Matrix->YAliens; j++)
		{
			if ((Matrix->matrix)[i][j] != NULL)
			{
				DrawEntity(Matrix->matrix[i][j]);
			}
		}
	}
}

void FillMatrix(AlienMatrix* Matrix , ALLEGRO_BITMAP * entityTexture)
{
	for (int i = 0; i < Matrix->XAliens; i++)
	{
		for (int j = 0; j < Matrix->YAliens; j++)
		{
			if ((Matrix->matrix)[i][j] == NULL)
			{
				(Matrix->matrix)[i][j] = CreateNewEntityLoadedTexture(
					NewVec2F(Matrix->Pos.x + (Matrix->AlienWidth + Matrix->AlienPaddingX) * i, Matrix->Pos.y + (Matrix->AlienHeight + Matrix->AlienPaddingY) * j),
					NewVec2F(0, 0), entityTexture, Matrix->AlienWidth, Matrix->AlienHeight);
			}
		}
	}

	Matrix->AlienCount = Matrix->XAliens * Matrix->YAliens;
}

void FillMatrixAnimated(AlienMatrix* Matrix, SpriteSheet * SpriteSheet_p)
{
	for (int i = 0; i < Matrix->XAliens; i++)
	{
		for (int j = 0; j < Matrix->YAliens; j++)
		{
			if ((Matrix->matrix)[i][j] == NULL)
			{
				(Matrix->matrix)[i][j] = CreateNewAnimatedEntityLoadedTexture(
					NewVec2F(Matrix->Pos.x + (Matrix->AlienWidth + Matrix->AlienPaddingX) * i, Matrix->Pos.y + (Matrix->AlienHeight + Matrix->AlienPaddingY) * j),
					NewVec2F(0, 0), SpriteSheet_p, Matrix->AlienWidth, Matrix->AlienHeight);
			}
		}
	}

	Matrix->AlienCount = Matrix->XAliens * Matrix->YAliens;
}

int CollideGrid(Entity* Bullet[], AlienMatrix* Matrix)
{
	int death=0;
	for (int b = 0; b < 10; b++)
	{
		for (int i = 0; i < Matrix->XAliens; i++)
		{
			for (int j = 0; j < Matrix->YAliens; j++)
			{
				if (Bullet[b] != NULL && (Matrix->matrix)[i][j] != NULL)
				{
					if (AreColiding((Matrix->matrix)[i][j], Bullet[b]))
					{
						DestroyEntityLoadedTexture(Bullet[b]);
						Bullet[b] = NULL;

						DestroyEntityLoadedTexture((Matrix->matrix)[i][j]);
						(Matrix->matrix)[i][j] = NULL;

						Matrix->AlienCount -= 1;
						death = 1;
					}
				}
			}
		}
	}
	return death;
}

void UpdateMatrix(AlienMatrix* Matrix, double dt , Vec2 PlayAreaPos , Vec2 PlayAreaDim)
{
	static double timeBuffer;
	timeBuffer = timeBuffer + dt;
	static int bounce;

	if (bounce)
	{
		for (int i = 0; i < Matrix->XAliens; i++)
		{
			for (int j = 0; j < Matrix->YAliens; j++)
			{
				if ((Matrix->matrix)[i][j] != NULL)
				{
					(Matrix->matrix)[i][j]->Pos.y += 3;
				}
			}
		}
		Matrix->HorizontalSpeed *= -1;
		Matrix->Pos.y += 3;
	}

	bounce = false;

	if (timeBuffer >=  ( 0.05 + 1.25 * ( Matrix->AlienCount / (double) ( Matrix->XAliens * Matrix->YAliens ) ) ))
	{
		timeBuffer = 0;
		Matrix->Pos.x += Matrix->HorizontalSpeed;
		
		for (int i = 0; i < Matrix->XAliens; i++)
		{
			for (int j = 0; j < Matrix->YAliens; j++)
			{
				if ((Matrix->matrix)[i][j] != NULL)
				{
					(Matrix->matrix)[i][j]->Pos.x += Matrix->HorizontalSpeed;

					if ((Matrix->matrix)[i][j]->Pos.x - (Matrix->AlienWidth/2) <= PlayAreaPos.x ||
						((Matrix->matrix)[i][j]->Pos.x + ( 1.5 * Matrix->AlienWidth) >= PlayAreaPos.x + PlayAreaDim.x))
					{
						bounce = true;
						
					}
				}
					
			}
		}
	}

	return;
}

void UpdateMatrixDynamic(AlienMatrix* Matrix, double dt, Vec2 PlayAreaPos, Vec2 PlayAreaDim)
{
	static double UpdateTimeBuffer;
	static double MicroTimeBuffer;

	static int updating;
	static int bounce;

	static int Xcoord;
	static int Ycoord;

	MicroTimeBuffer += dt;

	if (Xcoord >= Matrix->XAliens)
	{
		Xcoord = 0;
		Ycoord++;
	}
	if (Ycoord >= Matrix->YAliens)
	{
		Ycoord = 0;
		updating = false;
	}

	if (!updating)
	{
		UpdateTimeBuffer += dt;
	}

	if (UpdateTimeBuffer >= (0.05 + 1.25 * (Matrix->AlienCount / (double)(Matrix->XAliens * Matrix->YAliens))))
	{
		UpdateTimeBuffer = 0;
		updating = true;
	}

	if (updating)
	{
		if (MicroTimeBuffer >= 0.005)
		{
			MicroTimeBuffer = 0;

			int searching = true;
			int found = false;

			while (searching)
			{
				if (Matrix->matrix[Xcoord][Ycoord] == NULL)
				{
					Xcoord++;
					if (Xcoord >= Matrix->XAliens)
					{
						Xcoord = 0;
						Ycoord++;
						if (Ycoord >= Matrix->YAliens)
						{
							Ycoord = 0;
							searching = false;
							updating = false;
						}
					}
				}
				else
				{
					searching = false;
					found = true;
				}
			}
			if ( Matrix->matrix[Xcoord][Ycoord] != NULL && found)
			{
				Matrix->matrix[Xcoord][Ycoord]->Pos.x += Matrix->HorizontalSpeed;
				if ((Matrix->matrix)[Xcoord][Ycoord]->Pos.x - (Matrix->AlienWidth / 2) <= PlayAreaPos.x ||
					((Matrix->matrix)[Xcoord][Ycoord]->Pos.x + (1.5 * Matrix->AlienWidth) >= PlayAreaPos.x + PlayAreaDim.x))
				{
					bounce = true;

				}
				Xcoord++;
			}
			
		}
	}
	
	if (bounce && !updating)
	{
		Matrix->HorizontalSpeed *= -1;
		bounce = false;

		Matrix->Pos.y += 3;
		for (int i = 0; i < Matrix->XAliens; i++)
		{
			for (int j = 0; j < Matrix->YAliens; j++)
			{
				if (Matrix->matrix[i][j] != NULL)
				{
					Matrix->matrix[i][j]->Pos.y += 3;
				}
			}
		}

	}

}

Vec2F GetCentredPosition(AlienMatrix* Matrix, Vec2 ScreenDimension)
{
	return NewVec2F( ScreenDimension.x/2 - Matrix->width/2 , 50 );
}

void AnimateMatrix( AlienMatrix* Matrix , float dt )
{
	for (int i = 0; i < Matrix->XAliens; i++)
	{
		for (int j = 0; j < Matrix->YAliens; j++)
		{
			if (Matrix->matrix[i][j] != NULL)
			{
				Matrix->matrix[i][j]->deltaFrame += dt;
				if (Matrix->matrix[i][j]->deltaFrame + dt >= Matrix->matrix[i][j]->spriteS->maxDeltaFrame)
				{
					Matrix->matrix[i][j]->deltaFrame = 0;
					Matrix->matrix[i][j]->frameCount++;
					if (Matrix->matrix[i][j]->frameCount >= Matrix->matrix[i][j]->spriteS->maxFrameCount)
					{
						Matrix->matrix[i][j]->frameCount = 0;
					}
				}
			}
		}
	}
}