#include "Game.h"

void Game()
{
	GameInit();

	while (running)
	{
		GameLoop();
	}

	GameDestroy();

	return;
}

int GameInit()
{
	al_init();
	
	al_init_image_addon();

	al_install_keyboard();
	al_install_mouse();

	int error = 0;

	DISPLAY = al_create_display(1366, 768);
	if (DISPLAY == NULL)
	{
		printf("There has been an error with the display initialization");
		error = 1;
	}

	KeyboardEventSource = al_get_keyboard_event_source();
	if (KeyboardEventSource == NULL)
	{
		printf("There has been an error with the keyboard event source initialization");
		error = 1;
	}

	MouseEventSource = al_get_mouse_event_source();
	if (MouseEventSource == NULL)
	{
		printf("There has been an error with the mouse event source initialization");
		error = 1;
	}

	InputEventQueue = al_create_event_queue();
	if (InputEventQueue == NULL)
	{
		printf("There has been an error with the creation of the input event queue");
		error = 1;
	}

	al_register_event_source(InputEventQueue, KeyboardEventSource);
	al_register_event_source(InputEventQueue, MouseEventSource);

	Spaceship.Texture = al_load_bitmap("Resources/Ship.png");
	Spaceship.Pos = NewVec2( 300 , 300 );
	al_convert_mask_to_alpha(Spaceship.Texture, al_map_rgb(255, 0, 255));
	return;
}

void GameDestroy()
{
	al_shutdown_image_addon();

	al_destroy_display(DISPLAY);
	al_destroy_user_event_source(KeyboardEventSource);
	al_destroy_user_event_source(MouseEventSource);
	al_destroy_event_queue(InputEventQueue);

	return;
}

void GameLoop()
{
	Prechecks();
	GameLogic();
	GameRender();
}

void GameLogic()
{
	if (!al_is_event_queue_empty(InputEventQueue))
	{
		al_get_next_event(InputEventQueue, &TempEvent);

		switch (TempEvent.type)
		{
		case ALLEGRO_KEYBOARD_EVENT:

		}
		
	}


	return;
}

void GameRender()
{
	al_clear_to_color(al_map_rgb(0, 0,20));

	DrawEntity(&Spaceship);

	al_flip_display();
	return;
}

void Prechecks()
{
	

	return;
}