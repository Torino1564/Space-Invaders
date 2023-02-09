#pragma once

#include <allegro5/allegro5.h>
#include <allegro5/allegro.h>

void initInput();

int isPressingKey(int key);

ALLEGRO_KEYBOARD_STATE* TempState;