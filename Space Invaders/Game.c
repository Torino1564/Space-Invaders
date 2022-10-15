#include "Game.h"

void Game()
{
	SystemInit();

	int error = Menu();

	int errorI = GameInit();

	while (running )
	{
		GameLoop();
	}

	GameDestroy();

	return;
}

int SystemInit()
{
	int error = 0;

	al_init();

	al_init_image_addon();

	al_install_keyboard();
	al_install_mouse();

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

	PastFrameTime = 0;
	DeltaTime = 0;
}

int Menu()
{
	int error = 0;

	menu = al_load_bitmap(MENU_BG);
	start = al_load_bitmap(START_BG);
	if (menu == NULL)
	{
		return -1;
	}

	bool ready = 0;
	float width = al_get_bitmap_width(menu);
	float height = al_get_bitmap_height(menu);


	while (!ready)
	{
		int margin = 40;
		al_draw_scaled_rotated_bitmap(menu, width, height, ScreenDimensions.x, ScreenDimensions.y, ScreenDimensions.x/width, ScreenDimensions.y/height, 0, NULL);
		float width = al_get_bitmap_width(start);
		float height = al_get_bitmap_height(start);
		al_draw_scaled_rotated_bitmap(start, width, height, ScreenDimensions.x/3 - margin, ScreenDimensions.y -50, 1, 1, 0, NULL);


		if (!al_is_event_queue_empty(InputEventQueue))
		{
			al_get_next_event(InputEventQueue, &MenuEvent);

			switch (MenuEvent.type)
			{
			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
				if ((MenuEvent.mouse.x < ScreenDimensions.x / 3 - margin) && (MenuEvent.mouse.x > (ScreenDimensions.x / 3 - margin -width)
					&& (MenuEvent.mouse.y < (ScreenDimensions.y - 50)) && (MenuEvent.mouse.y > (ScreenDimensions.y - 50 - height)))
					)
					{
						ready = 1;
					}
			}			

		}

		al_flip_display();

	}

}

int GameInit()
{
	int error = 0;

	Vec2 GridDimensions = NewVec2(768, 240);
	int AlienPaddingX = (GridDimensions.x - 50 * 11) / 10;
	int AlienPaddingY = (GridDimensions.y - 50 * 4) / 3;
	XAliens = 11;
	YAliens = 4;
	int AlienWidth = 50;
	int AlienHeight = 50;

	AlienTexture = al_load_bitmap(ALIEN_TEXTURE1);
	if (AlienTexture == NULL)
	{
		printf("There has been an error loading the alien texture");
		error = -1;
	}

	AlienGrid = NewMatrix( NewVec2F(ScreenDimensions.x/2 - GridDimensions.x/2, 50), GridDimensions.x, GridDimensions.y, AlienWidth, AlienHeight , 50);
	if (AlienGrid == NULL)
	{
		printf("There has been an error creating the Alien Matrix");
		error = -2;
	}
	else if (AlienTexture != NULL)
	{

		for (int i = 0; i < YAliens; i++)
		{
			for (int j = 0; j < XAliens; j++)
			{
				 AlienGrid->matrix[i][j] = CreateNewEntityLoadedTexture(
					NewVec2F(AlienGrid->Pos.x + (50 + AlienGrid->AlienPaddingX) * j, AlienGrid->Pos.y + (50 + AlienGrid->AlienPaddingY) * i),
					NewVec2F(0, 0), AlienTexture , 50, 50);

			}

		}
		
		AlienGrid->AlienCount = 0; //Esta linea esta para que la funcion pase por AlienSpawn por primera vez
	}
	
	background1 = al_load_bitmap(LVL1_BG);
	if (background1 == NULL)
	{
		printf("There has ben an error loading the level 1 background bitmap");
		return -1;
	}

	Spaceship = CreateNewEntity(NewVec2F(ScreenDimensions.x/2 - 50/2, ScreenDimensions.y - 80 ), NewVec2F(0, 0), SHIP_TEXTURE, 40, 50);
	if (Spaceship == NULL)
	{
		printf("There has been an error creating the player spaceship");
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
	BulletTexture = al_load_bitmap(BULLET_TEXTURE1);

	return error;
}

void GameDestroy()
{
	DestroyMatrix(AlienGrid);
	DestroyEntity(Spaceship);
	DestroyEntity(Alien);
	al_destroy_bitmap(menu);
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
							Bullets[i] = CreateNewEntityLoadedTexture(NewVec2F((int)(Spaceship->Pos.x) + Spaceship->width / 2 - 20 / 2, Spaceship->Pos.y - 20), NewVec2F(0, -600), BulletTexture , 5, 20);
							break;
						}

						if (Bullets[i] == NULL)
						{
							Bullets[i] = CreateNewEntityLoadedTexture(NewVec2F((int)(Spaceship->Pos.x) + Spaceship->width / 2 - 20 / 2, Spaceship->Pos.y - 20), NewVec2F(0, -600), BulletTexture, 5, 20);
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

	UpdateMatrix(AlienGrid, PastFrameTime , ScreenDimensions);

	CullBullets();
	UpdateBullets();

	CollideGrid(Bullets, AlienGrid);

	UpdateEntity(Spaceship, DeltaTime);
	ClipToScreen(Spaceship, ScreenDimensions);


	return;
}

void GameRender()
{
	al_draw_scaled_bitmap(background1, 0, 0, al_get_bitmap_width(background1), al_get_bitmap_height(background1), 0, 0, ScreenDimensions.x, ScreenDimensions.y, NULL);

	DrawEntity(Spaceship);

	if (AlienGrid->AlienCount == 0)
	{
		for (int i = 0; i < 10; i++) //Limpia las balas que quedan volando cuando se quedan sin aliens
		{
			if (Bullets[i] != NULL)
			{
				DestroyEntityLoadedTexture(Bullets[i]);
				Bullets[i] = NULL;
			}
		}
		SpawnMatrix(AlienGrid, AlienTexture);
		t = clock();
	}

	DrawGrid(AlienGrid);

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
	PastFrameTime = DeltaTime;
	DeltaTime = (double)(t) / (CLOCKS_PER_SEC);

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
				DestroyEntityLoadedTexture(Bullets[i]);
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