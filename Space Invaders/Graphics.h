#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "Vec2.h"

ALLEGRO_DISPLAY* Display;
char Grid[16][16];

void PrintGrid();
void TurnOn(int x, int y);
void ClearGrid();