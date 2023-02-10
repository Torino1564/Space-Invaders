#pragma once
#ifndef RASPI_TEST
#include <allegro5/allegro.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

ALLEGRO_DISPLAY* Display;
char Grid[16][16];
#endif

#include "Vec2.h"


#ifdef RASPI_TEST
#include "Resources/libs/joydisp/disdrv.h"
#endif



void InitGraphics();
void PrintGrid();
void TurnOn(int x, int y);
void ClearGrid();