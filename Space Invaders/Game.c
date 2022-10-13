#include "Game.h"

void Game()
{

	/* Aca iria el menu*/

	int error = GameInit();

	while (running && !error)
	{
		GameLoop();
	}

	GameDestroy();

	return;
}

int GameInit()
{
	int error = 0;

	al_init();
	
	al_init_image_addon();

	al_install_keyboard();
	al_install_mouse();

	
	menu0 = al_load_bitmap("Metal_Slug_X-title.png");
	menu1 = al_create_bitmap(100, 100);
	
	if (menu1 != NULL)
	{
		al_draw_bitmap(menu1, 100, 100, NULL);
	}
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

	Vec2 GridDimensions = NewVec2(768, 240);
	int AlienPaddingX = (GridDimensions.x - 50 * 11) / 10;
	int AlienPaddingY = (GridDimensions.y - 50 * 4) / 3;
	XAliens = 11;
	YAliens = 4;
	int AlienWidth = 50;
	int AlienHeight = 50;


	AlienGrid = NewMatrix( NewVec2F(ScreenDimensions.x/2 - GridDimensions.x/2, 50), GridDimensions.x, GridDimensions.y, AlienWidth, AlienHeight);
	if (AlienGrid == NULL)
	{
		printf("There has been an error creating the Alien Matrix");
		error = -2;
	}
	else
	{
		for (int i = 0; i < YAliens; i++)
		{
			for (int j = 0; j < XAliens; j++)
			{
				 AlienGrid->matrix[i][j] = CreateNewEntity(
					NewVec2F(AlienGrid->Pos.x + (50 + AlienPaddingX) * j, AlienGrid->Pos.y + (50 + AlienPaddingY) * i),
					NewVec2F(0, 0), "Resources/alien1.png", 50, 50);

			}

		}
		
	}
	

	Spaceship = CreateNewEntity(NewVec2F(600, 600), NewVec2F(0, 0), "Resources/Ship.png", 40, 50);
	if (Spaceship == NULL)
	{
		printf("There has been an error creating the player spaceship");
		error = -1;
	}

	Alien = CreateNewEntity(NewVec2F(100, 50), NewVec2F(0, 0), "Resources/alien1.png", 50, 50);
	if (Alien == NULL)
	{
		printf("There has been an error creating the Alien entity");
		error = -1;
	}
	

	Bullets[0] = malloc(sizeof(Entity) * 10);
	if (Bullets[0] != NULL)
	{
		for (int i = 0; i < 10; i++)
		{
			Bullets[i] = NULL;
		}
	}


	return error;
}

void GameDestroy()
{
	DestroyMatrix(AlienGrid);
	DestroyEntity(Spaceship);
	DestroyEntity(Alien);
	al_destroy_bitmap(menu0);
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
					Spaceship->Vel.x -= SHIP_SPEED;
					break;
				case ALLEGRO_KEY_D:
					Spaceship->Vel.x += SHIP_SPEED;
					break;
				/*case ALLEGRO_KEY_W:
					spaceship->vel.y -= ship_speed;
					break;
				case ALLEGRO_KEY_S:
					spaceship->vel.y += ship_speed;
					break;*/
				case ALLEGRO_KEY_F4:
					if (al_key_down(&KeyboardCurrentState, ALLEGRO_KEY_ALT))
						running = 0;
					break;
				case ALLEGRO_KEY_SPACE:
					for (int i = 0; i < 10; i++)
					{
						if (i == 9)
						{
							Bullets[i] = CreateNewEntity(NewVec2F(Spaceship->Pos.x + Spaceship->width / 2 - 10 / 2, Spaceship->Pos.y), NewVec2F(0, -600), "Resources/Bullet.png", 10, 10);
							break;
						}

						if (Bullets[i] == NULL)
						{
							Bullets[i] = CreateNewEntity(NewVec2F(Spaceship->Pos.x + Spaceship->width / 2 - 10 / 2, Spaceship->Pos.y), NewVec2F(0, -600), "Resources/Bullet.png", 10, 10);
							break;
						}
					}
						break;
				default:
					break;

			}
			break;
		case ALLEGRO_EVENT_KEY_UP:
			switch (TempEvent.keyboard.keycode)
			{
			case ALLEGRO_KEY_A:
				Spaceship->Vel.x += SHIP_SPEED;
				break;
			case ALLEGRO_KEY_D:
				Spaceship->Vel.x -= SHIP_SPEED;
				break;
			/*case ALLEGRO_KEY_W:
				Spaceship->Vel.y += SHIP_SPEED;
				break;
			case ALLEGRO_KEY_S:
				Spaceship->Vel.y -= SHIP_SPEED;
				break;*/
			default:
				break;

			}
			break;


		}
		
	}

	CullBullets();
	UpdateBullets();

	UpdateEntity(Spaceship, DeltaTime);
	ClipToScreen(Spaceship, ScreenDimensions);


	return;
}

void GameRender()
{
	al_clear_to_color(al_map_rgb(0, 0,10));

	DrawEntity(Spaceship);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			if ((* (AlienGrid->matrix) + i + j*XAliens) != NULL)
			{
				DrawEntity(AlienGrid->matrix[i][j]);
			}
		}
	}

	for (int i = 0; i < 10; i++)
	{
		if (Bullets[i] != NULL)
		{
			DrawEntity(Bullets[i]);
		}
	}

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

void CullBullets()
{
	for (int i = 0; i < 10; i++)
	{
		if (Bullets[i] != NULL)
		{
			if (Bullets[i]->Pos.y < 0)
			{
				DestroyEntity(Bullets[i]);
				Bullets[i] = NULL;
			}
		}
	}
}

void UpdateBullets()
{
	for (int i = 0; i < 10; i++)
	{
		if (Bullets[i] != NULL)
		{
			UpdateEntity(Bullets[i], DeltaTime);
		}
	}
}