#pragma once
#include "AlienMatrix.h"
#ifndef RASPI
AlienMatrix* NewMatrix( int AlienPaddingX, int AlienPaddingY , int Awidth_p , int Aheight_p, int XAliens_p, int YAliens_p, int hspeed, int vspeed)
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

		TempMatrix->HorizontalSpeed = hspeed;
		TempMatrix->VerticalSpeed = vspeed;

		TempMatrix->MicroTimeBuffer = 0.005;

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
				DestroyAnimatedEntitySharedSprite(matrix->matrix[i][j]);
				matrix->matrix[i][j] = NULL;
			}
		}
	}
	free(matrix);
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

int CollideGrid(Entity* Bullet[], AlienMatrix* Matrix, int * aliensDestroyed, SpriteSheet* ExplosionSpritesheet)
{
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

						CreateNewAnimation( NewVec2F((Matrix->matrix)[i][j]->Pos.x + (float)Matrix->AlienWidth/2.0 - ExplosionSpritesheet->frameWidth * (3.0 / 4.0),
							(Matrix->matrix)[i][j]->Pos.y + (float)Matrix->AlienHeight/2.0 - ExplosionSpritesheet->frameHeight * (3.0/4.0))
							, NewVec2F(0, 5), 0, ExplosionSpritesheet, Matrix->AlienWidth * 1.9, Matrix->AlienHeight * 1.9);

						DestroyEntityLoadedTexture((Matrix->matrix)[i][j]);
						(Matrix->matrix)[i][j] = NULL;
						
						Matrix->AlienCount--;

						(*aliensDestroyed)++;
						
						return true;
					}
				}
			}
		}
	}
	return false;
}

int CollideEntity(Entity* BigUFOent, Entity* Bullets[])
{
	for (int b = 0; b < 10; b++)
	{
		if (*(Bullets + b) != NULL && BigUFOent != NULL)
		{
			if (AreColiding(BigUFOent, Bullets[b]))
			{
				DestroyEntityLoadedTexture(Bullets[b]);
				Bullets[b] = NULL;


				DestroyAnimatedEntitySharedSprite(BigUFOent);
				BigUFOent = NULL;

				return true;
			}
		}
	}
	return false;
}

void DeathAnimation(int i, int j)
{
	return;
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

					if ((Matrix->matrix)[i][j]->Pos.x - ((float)Matrix->AlienWidth/2.0) <= PlayAreaPos.x ||
						((Matrix->matrix)[i][j]->Pos.x + ( 1.5 * (float)Matrix->AlienWidth) >= PlayAreaPos.x + PlayAreaDim.x))
					{
						bounce = true;
						
					}
				}
					
			}
		}
	}

	return;
}



Vec2F GetCentredPosition(AlienMatrix* Matrix, Vec2F ScreenDimension)
{
	return NewVec2F( ScreenDimension.x/2 - (float)Matrix->width/2.0 , 50 );
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

void FillMatrixAnimated(AlienMatrix* Matrix, SpriteSheet* SpriteSheet_p)
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
#endif

#ifdef RASPI
AlienMatrix* CreateNewAlienMatrix(Vec2 Pos, int HorizontalSpeed, int XAliens, int YAliens, Vec2 AlienDimensions, int Padding, char shape[])
{

	AlienMatrix* TempMatrix = malloc(sizeof(AlienMatrix));
	if (TempMatrix == NULL)  return NULL;

	TempMatrix->Pos = Pos;
	TempMatrix->HorizontalSpeed = HorizontalSpeed;
	TempMatrix->VerticalSpeed = 1;

	TempMatrix->XAliens = XAliens;
	TempMatrix->YAliens = YAliens;
	TempMatrix->AlienDimensions = AlienDimensions;
	TempMatrix->AlienPadding = Padding;

	TempMatrix->MicroTimeBuffer = 0.05;
	
	for (int i = 0; i < 25; i++)
	{
		if (i < AlienDimensions.x * AlienDimensions.y)
		{
			TempMatrix->Shape[i] = shape[i];
		}
		else
		{
			TempMatrix->Shape[i] = 0;
		}
	}


	for (int i = 0; i < XAliens; i++)
	{
		for (int j = 0; j < YAliens; j++)
		{
			TempMatrix->matrix[i][j] = NULL;
		}
	}

	return TempMatrix;

}

void DestroyAlienMatrix(AlienMatrix* Matrix)
{
	for (int i = 0; i < Matrix->XAliens; i++)
	{
		for (int j = 0; j < Matrix->YAliens; j++)
		{
			if (Matrix->matrix[i][j] != NULL)
			{
				DestroyEntity(Matrix->matrix[i][j]);
				Matrix->matrix[i][j] = NULL;
			}
		}
	}

	free(Matrix);
}

void FillMatrix(AlienMatrix* Matrix)
{
	for (int i = 0; i < Matrix->XAliens; i++)
	{
		for (int j = 0; j < Matrix->YAliens; j++)
		{
			if ((Matrix->matrix)[i][j] == NULL)
			{
				Matrix->matrix[i][j] = CreateNewEntity(NewVec2(Matrix->Pos.x + (Matrix->AlienDimensions.x + Matrix->AlienPadding) * i,
					Matrix->Pos.y + (Matrix->AlienDimensions.y + Matrix->AlienPadding) * j), NewVec2(0, 0), 0, Matrix->Shape, Matrix->AlienDimensions);

			}
		}
	}

	Matrix->AlienCount = Matrix->XAliens * Matrix->YAliens;
}

int CollideGrid(Entity* Bullet[], AlienMatrix* Matrix, int* aliensDestroyed)
{
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
						DestroyEntity(Bullet[b]);
						Bullet[b] = NULL;

						DestroyEntity((Matrix->matrix)[i][j]);
						(Matrix->matrix)[i][j] = NULL;

						(* aliensDestroyed)++;

						Matrix->AlienCount--;

						return true;
					}
				}
			}
		}
	}
	return false;
}

#endif

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

int UpdateMatrixDynamic(AlienMatrix* Matrix, double dt, Vec2 PlayAreaPos, Vec2 PlayAreaDim, Entity* Spaceship)
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

	if (UpdateTimeBuffer >= ((0.05 + 1.25 * (Matrix->AlienCount / (double)(Matrix->XAliens * Matrix->YAliens))) * TIME_MULTIPLIER))
	{
		UpdateTimeBuffer = 0;
		updating = true;
	}

	if (updating)
	{
		if (MicroTimeBuffer >= Matrix->MicroTimeBuffer * TIME_MULTIPLIER)
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
			if (Matrix->matrix[Xcoord][Ycoord] != NULL && found)
			{
				Matrix->matrix[Xcoord][Ycoord]->Pos.x += Matrix->HorizontalSpeed;
#ifndef RASPI
				if ((Matrix->matrix)[Xcoord][Ycoord]->Pos.x - ((float)Matrix->AlienWidth / 2.0) <= PlayAreaPos.x ||
					((Matrix->matrix)[Xcoord][Ycoord]->Pos.x + (1.5 * Matrix->AlienWidth) >= PlayAreaPos.x + PlayAreaDim.x))
				{
					bounce = true;

				}
				static int once = 1;
				if ((Matrix->AlienCount <= 15) && (once == 1))
				{
					Matrix->HorizontalSpeed += 10;
					once = 2;
				}
				if ((Matrix->AlienCount <= 10) && (once == 2))
				{
					Matrix->HorizontalSpeed += 10;
					once = 3;
				}
				if ((Matrix->AlienCount <= 5) && (once == 3))
				{
					Matrix->HorizontalSpeed += 10;
					once = 4;
				}
				if ((Matrix->AlienCount <= 2) && (once == 4))
				{
					Matrix->HorizontalSpeed += 10;
					once = 5;
				}
				if ((Matrix->AlienCount <= 1) && (once == 5))
				{
					Matrix->HorizontalSpeed += 10;
					once = 6;
				}
				if ((Matrix->AlienCount == 0) && (once == 2))
				{
					Matrix->HorizontalSpeed += 10;
					once = 1;
				}
#endif
#ifdef RASPI
				if (Matrix->matrix[Xcoord][Ycoord]->Pos.x + Matrix->matrix[Xcoord][Ycoord]->dimensions.x >= 16 ||
					Matrix->matrix[Xcoord][Ycoord]->Pos.x <= 0)
				{
					bounce = true;
				}
#endif


				Xcoord++;
			}

		}
	}

	if (bounce && !updating)
	{
		Matrix->HorizontalSpeed *= -1;
		bounce = false;

		Matrix->Pos.y += Matrix->VerticalSpeed;
		for (int i = 0; i < Matrix->XAliens; i++)
		{
			for (int j = 0; j < Matrix->YAliens; j++)
			{
				if (Matrix->matrix[i][j] != NULL)
				{
					Matrix->matrix[i][j]->Pos.y += Matrix->VerticalSpeed;
					if (Matrix->matrix[i][j]->Pos.y + Matrix->matrix[i][j]->height >= Spaceship->Pos.y - 250)
					{
						return false;
					}
				}
			}
		}

	}
	return true;
}