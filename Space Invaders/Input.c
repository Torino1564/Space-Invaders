#include "Input.h"

void initInput()
{
#ifndef RASPI_TEST
	al_install_keyboard();

	TempState = malloc(sizeof(ALLEGRO_KEYBOARD_STATE));
#endif
#ifdef RASPI_TEST
	joy_init();
#endif
}

int isPressingKey(int key)
{
#ifndef RASPI_TEST
	switch (key)
	{
	case LEFT:
		key = ALLEGRO_KEY_A;
		break;
	case RIGHT:
		key = ALLEGRO_KEY_D;
		break;
	case SHOOT:
		key = ALLEGRO_KEY_SPACE;
		break;
	}
	al_get_keyboard_state(TempState);

	return al_key_down(TempState, key);
#endif
#ifdef RASPI_TEST
	joy_update();
	jcoord_t TempCoords = joy_get_coord();
	jswitch_t TempSwitch = joy_get_switch();

	switch (key)
	{
		case LEFT:
		{
			if (TempCoords.x < 0) return 1;
			break;
		}
		case RIGHT:
		{
			if (TempCoords.x > 0) return 1;
			break;
		}
		case SHOOT:
		{
			if (TempSwitch = J_PRESS)
			{
				return 1;
			}
			break;
		}
	}
	return 0;
	
#endif
}
