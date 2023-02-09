#include "Input.h"

void initInput()
{
	al_install_keyboard();

	TempState = malloc(sizeof(ALLEGRO_KEYBOARD_STATE));
}

int isPressingKey(int key)
{
	al_get_keyboard_state(TempState);

	return al_key_down(TempState, key);
}
