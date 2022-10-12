#include "Game.h"

void Game()
{
	int error = GameInit();

	while (running && !error)
	{
		GameLoop();
	}

	if (!running)
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

	ScreenDimensions = NewVec2(1366, 768);

	DISPLAY = al_create_display(ScreenDimensions.x, ScreenDimensions.y);
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
	Spaceship.Pos = NewVec2F( 600 , 600 );
	al_convert_mask_to_alpha(Spaceship.Texture, al_map_rgb(255, 0, 255));
	Spaceship.Vel = NewVec2F(0, 0);
	Spaceship.width = 50;
	Spaceship.height = 40;

	/*Alien = CreateNewEntity(NewVec2F(100, 50), NewVec2F(0, 0), "Resources/alien1.png", 50, 50);
	if (Alien == NULL)
	{
		printf("There has been an error creating the Alien entity");
		error = -1;
	}
	*/

	Alien.height = 50;
	Alien.width = 50;
	Alien.Pos = NewVec2F(100, 50);
	Alien.Vel = NewVec2F(0, 0);
	Alien.Texture = al_load_bitmap("Resources/alien1.png");
	al_convert_mask_to_alpha(Alien.Texture, al_map_rgb(255, 0, 255));

	return error;
}

void GameDestroy()
{

	al_destroy_bitmap(Spaceship.Texture);

	al_destroy_display(DISPLAY);
	al_destroy_user_event_source(KeyboardEventSource);
	al_destroy_user_event_source(MouseEventSource);
	al_destroy_event_queue(InputEventQueue);

	al_shutdown_image_addon();

	return;
}

void GameLoop()
{
	Preframe();
	GameLogic();
	GameRender();
	Postframe();
}

void GameLogic()
{
	if (!al_is_event_queue_empty(InputEventQueue))
	{
		al_get_next_event(InputEventQueue, &TempEvent);

		switch (TempEvent.type)
		{
		case ALLEGRO_EVENT_KEY_DOWN:
			switch (TempEvent.keyboard.keycode)
			{
				case ALLEGRO_KEY_A:
					Spaceship.Vel.x -= SHIP_SPEED;
					break;
				case ALLEGRO_KEY_D:
					Spaceship.Vel.x += SHIP_SPEED;
					break;
				case ALLEGRO_KEY_W:
					Spaceship.Vel.y -= SHIP_SPEED;
					break;
				case ALLEGRO_KEY_S:
					Spaceship.Vel.y += SHIP_SPEED;
					break;
				case ALLEGRO_KEY_F4:
					if (al_key_down(&KeyboardCurrentState, ALLEGRO_KEY_ALT))
						running = 0;
					break;
				default:
					break;

			}
			break;
		case ALLEGRO_EVENT_KEY_UP:
			switch (TempEvent.keyboard.keycode)
			{
			case ALLEGRO_KEY_A:
				Spaceship.Vel.x += SHIP_SPEED;
				break;
			case ALLEGRO_KEY_D:
				Spaceship.Vel.x -= SHIP_SPEED;
				break;
			case ALLEGRO_KEY_W:
				Spaceship.Vel.y += SHIP_SPEED;
				break;
			case ALLEGRO_KEY_S:
				Spaceship.Vel.y -= SHIP_SPEED;
				break;
			default:
				break;

			}
			break;


		}
		
	}

	UpdateEntity(&Spaceship, DeltaTime);
	ClipToScreen(&Spaceship, ScreenDimensions);
	if (AreColiding(&Spaceship, &Alien))
		running = 0;

	return;
}

void GameRender()
{
	al_clear_to_color(al_map_rgb(0, 0,20));

	DrawEntity(&Spaceship);

	DrawEntity(&Alien);

	al_flip_display();
	return;
}

void Preframe()
{
	al_get_keyboard_state(&KeyboardCurrentState);

	t = clock();

	return;
}

void Postframe()
{
	t = clock() - t;
	DeltaTime = ((double)t) / CLOCKS_PER_SEC;

	return;
}