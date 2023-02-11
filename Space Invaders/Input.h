#pragma once
#ifdef RASPI_TEST
#include "Resources/libs/joydisp/joydrv.h"
#endif

#ifndef RASPI_TEST
#include <allegro5/allegro5.h>
#include <allegro5/allegro.h>

ALLEGRO_KEYBOARD_STATE* TempState;
#endif

enum Input { LEFT, RIGHT, UP, DOWN, SHOOT };

void initInput();

int isPressingKey(int key);

