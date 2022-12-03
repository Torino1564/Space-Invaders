#include "Game.h"

void Game()
{
	SystemInit();

	int error = Menu();

	int errorI = GameInit();

	while (running)
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

	al_install_audio();
	al_init_acodec_addon();

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


	Mixer = al_get_default_mixer();

	al_reserve_samples(SAMPLE_COUNT);

}

int Menu()
{
	int error = 0;

	menu = al_load_bitmap(MENU_BG);
	if (menu == NULL)
	{
		return -1;
	}

	start = al_load_bitmap(START_BG);
	if (start == NULL)
	{
		return -1;
	}

	bool ready = 0;
	float width = al_get_bitmap_width(menu);
	float height = al_get_bitmap_height(menu);

	menuMusic = al_load_sample(BACKGROUNDMUSIC1);

	al_play_sample(menuMusic, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);

//	ALLEGRO_SAMPLE_INSTANCE* playMenuMusic = al_create_sample_instance(menuMusic);
//	al_set_sample_instance_playmode(playMenuMusic, ALLEGRO_PLAYMODE_LOOP);
//	al_attach_sample_instance_to_mixer(playMenuMusic, Mixer);

//	al_play_sample_instance(playMenuMusic);


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
	al_stop_samples();

}

int GameInit()
{
	int error = 0;

	GridDimensions = NewVec2(600, 220);
	int AlienPaddingX = (GridDimensions.x - 50 * 11) / 10;
	int AlienPaddingY = (GridDimensions.y - 50 * 4) / 3;
	XAliens = 11;
	YAliens = 5;
	AlienWidth = 40;
	AlienHeight = 40;

	PlaySpaceArea = NewVec2(900, 700);
	PlaySpacePos = NewVec2(ScreenDimensions.x / 2 - PlaySpaceArea.x / 2, 20);
	GUIColor = al_map_rgb(40, 60, 20);

	AlienTexture = al_load_bitmap(ALIEN_TEXTURE1);
	if (AlienTexture == NULL)
	{
		printf("There has been an error loading the alien texture");
		error = -1;
	}

	AlienGrid = NewMatrix( NewVec2F(ScreenDimensions.x/2 - GridDimensions.x/2 - AlienWidth, 50), GridDimensions.x, GridDimensions.y, AlienWidth, AlienHeight , 15 );
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
					NewVec2F(0, 0), AlienTexture , 40, 40);

			}

		}
		
		AlienGrid->AlienCount = 0; //Esta linea esta para que la funcion pase por AlienSpawn por primera vez
	}
	
	background1 = al_load_bitmap(LVL1_BG);
	background2 = al_load_bitmap(LVL2_BG);
	background4 = al_load_bitmap(LVL4_BG);
	if (background2 == NULL)
	{
		printf("There has ben an error loading the level 4 background bitmap");
		return -1;
	}

	if (background4 == NULL)
	{
		printf("There has ben an error loading the level 4 background bitmap");
		return -1;
	}

	level1Music = al_load_sample(MUSIC_LEVEL1);
	level2Music = al_load_sample(MUSIC_LEVEL2);
	level4Music = al_load_sample(MUSIC_LEVEL4);



	Spaceship = CreateNewEntity(NewVec2F(ScreenDimensions.x/2 - 50/2, ScreenDimensions.y - 80 ), NewVec2F(0, 0), SHIP_TEXTURE, 40, 50);
	if (Spaceship == NULL)
	{
		printf("There has been an error creating the player spaceship");
		error = -1;
	}

	Bullets[0] = malloc(sizeof(Entity) * 10);
	if (Bullets != NULL)
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
	al_destroy_bitmap(menu);
	al_destroy_display(DISPLAY);
	al_destroy_user_event_source(KeyboardEventSource);
	al_destroy_user_event_source(MouseEventSource);
	al_destroy_event_queue(InputEventQueue);
	al_destroy_sample(BackgroundMusic1);

	al_uninstall_keyboard();
	al_uninstall_mouse();
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
				case ALLEGRO_KEY_S:
					Level += 1;
					break;

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

	if (AlienGrid->AlienCount == 0)
	{
		Level += 1;
	}


	return;
}

void GameRender()
{

	//Background
	switch (Level)
	{
		case 1:
			al_draw_scaled_bitmap(background1, 0, 0, al_get_bitmap_width(background1), al_get_bitmap_height(background1), 0, 0, ScreenDimensions.x, ScreenDimensions.y, 4, 11, NULL);
			break;
		case 2:
			al_draw_scaled_bitmap(background2, 0, 0, al_get_bitmap_width(background2), al_get_bitmap_height(background2), 0, 0, ScreenDimensions.x, ScreenDimensions.y, 4, 11, NULL);
			break;
		case 3:
			al_draw_scaled_bitmap(background4, 0, 0, al_get_bitmap_width(background4), al_get_bitmap_height(background4), 0, 0, ScreenDimensions.x, ScreenDimensions.y, 4, 11, NULL);
			break;
		case 4:
			al_draw_scaled_bitmap(background4, 0, 0, al_get_bitmap_width(background4), al_get_bitmap_height(background4), 0, 0, ScreenDimensions.x, ScreenDimensions.y, 4, 11, NULL);
			break;
	}
	
	//Music
	static int playing = 0;
	
	if (playing == 0) //primer nivel
	{
		al_stop_samples();
		al_play_sample(level1Music, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
		playing = Level;
	}
	if (playing == 1)  //espera al segundo nivel
	{
		playing = Level;
	}
	if (playing == 2) //segundo nivel
	{
		al_stop_samples();
		al_play_sample(level2Music, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
		playing = Level+1;
	}
	if (playing == 3) // espera al tercer nivel
	{
		playing = Level+1;
	}
	if (playing == 4) // tercer nivel
	{
		al_stop_samples();
		al_play_sample(level4Music, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
		playing = Level+2;
	}
	if (playing == 5) // espera al cuarto nivel
	{
		playing = Level+2;
	}
	if (playing == 6)
	{
		al_stop_samples();
		al_play_sample(level4Music, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
		playing = Level+5;
	}
	if (playing == 9)
	{
		playing = 3*Level-3;
	}
	
	//Render GUI
	for (int i = 0; i < ScreenDimensions.x; i++)
	{
		for (int j = 0; j < ScreenDimensions.y; j++)
		{
			if ((i >= PlaySpacePos.x && i <= PlaySpacePos.x + PlaySpaceArea.x) && (j >= PlaySpacePos.y && j <= PlaySpacePos.y + PlaySpaceArea.y))
			{
				//al_put_pixel(i, j, GUIColor);
			}

		}
	}
	//player
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