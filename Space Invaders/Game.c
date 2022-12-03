#include "Game.h"

void Game()
{
	SystemInit();

	int error = Menu();

	int errorI = GameInit();
	Level = 0;
	Once = 0;

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

	al_init_primitives_addon();

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

	int AlienPaddingX = 20;
	int AlienPaddingY = 10;
	XAliens = 11;
	YAliens = 5;
	AlienWidth = 45;
	AlienHeight = 45;

	PlaySpaceArea = NewVec2( 1000 , ScreenDimensions.y );
	PlaySpacePos = NewVec2(ScreenDimensions.x/2 - PlaySpaceArea.x/2, 0);
	GUIColor = al_map_rgb(40, 60, 20);

	AlienTexture = al_load_bitmap(ALIEN_TEXTURE1);
	if (AlienTexture == NULL)
	{
		printf("There has been an error loading the alien texture");
		error = -1;
	}

	AlienGrid = NewMatrix( AlienPaddingX, AlienPaddingY, AlienWidth, AlienHeight , XAliens, YAliens, 15 );
	if (AlienGrid == NULL)
	{
		printf("There has been an error creating the Alien Matrix");
		error = -2;
	}
	
	background1 = al_load_bitmap(LVL1_BG);
	background2 = al_load_bitmap(LVL2_BG);
	background4 = al_load_bitmap(LVL4_BG);

	if (background1 == NULL)
	{
		printf("There has ben an error loading the level 1 background bitmap");
		return -1;
	}
	if (background2 == NULL)
	{
		printf("There has ben an error loading the level 2 background bitmap");
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

	Spaceship = CreateNewEntity(NewVec2F(ScreenDimensions.x/2 - 50/2, PlaySpacePos.y + PlaySpaceArea.y - 80 ), NewVec2F(0, 0), SHIP_TEXTURE, 40, 50);
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

	MiniUFO = NewSpriteSheet(MINIUFO1SP, (float)((float)1 / (float)12), 16, 44, 38);

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

	al_shutdown_primitives_addon();
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
					Once = 0;
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

	UpdateMatrixDynamic(AlienGrid, PastFrameTime , PlaySpacePos , PlaySpaceArea);
	//UpdateMatrix(AlienGrid, PastFrameTime , PlaySpacePos , PlaySpaceArea);
	AnimateMatrix(AlienGrid, DeltaTime);

	CullBullets();
	UpdateBullets();

	CollideGrid(Bullets, AlienGrid);

	UpdateEntity(Spaceship, DeltaTime);
	ClipToScreen(Spaceship, ScreenDimensions);

	if (AlienGrid->AlienCount == 0)
	{
		Level += 1;	//Pasa al siguiente nivel
		Once = 0;	//Resetea para el sonido

		for (int i = 0; i < 10; i++) //Limpia las balas que quedan volando cuando se quedan sin aliens
		{
			if (Bullets[i] != NULL)
			{
				DestroyEntityLoadedTexture(Bullets[i]);
				Bullets[i] = NULL;
			}
		}
		AlienGrid->Pos = GetCentredPosition(AlienGrid, ScreenDimensions);
		//FillMatrix(AlienGrid, AlienTexture);
		FillMatrixAnimated(AlienGrid, MiniUFO);


	}
	return;
}

void GameRender()
{	
	//Background
	switch ((Level-1)%4)
	{
	case 0:
		al_draw_scaled_bitmap(background1, 0, 0, al_get_bitmap_width(background1), al_get_bitmap_height(background1), PlaySpacePos.x, PlaySpacePos.y, PlaySpaceArea.x, PlaySpaceArea.y, NULL);
		break;
	case 1:
		al_draw_scaled_bitmap(background2, 0, 0, al_get_bitmap_width(background2), al_get_bitmap_height(background2), PlaySpacePos.x, PlaySpacePos.y, PlaySpaceArea.x, PlaySpaceArea.y, NULL);
		break;
	case 2:
		al_draw_scaled_bitmap(background4, 0, 0, al_get_bitmap_width(background4), al_get_bitmap_height(background4), PlaySpacePos.x, PlaySpacePos.y, PlaySpaceArea.x, PlaySpaceArea.y, NULL);
		break;
	case 3:
		al_draw_scaled_bitmap(background4, 0, 0, al_get_bitmap_width(background4), al_get_bitmap_height(background4), PlaySpacePos.x, PlaySpacePos.y, PlaySpaceArea.x, PlaySpaceArea.y, NULL);
		break;
	}

	//Music
	
	switch ((Level - 1) % 4)
	{
		case 0:
			if (Once == 0)
			{
				al_stop_samples();
				al_play_sample(level1Music, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
				Once = 1;
			}
		case 1:
			if (Once == 0)
			{
				al_stop_samples();
				al_play_sample(level2Music, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
				Once = 1;
			}
		case 2:
			if (Once == 0)
			{
				al_stop_samples();
				al_play_sample(level2Music, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
				Once = 1;
			}
		case 3:
			if (Once == 0)
			{
				al_stop_samples();
				al_play_sample(level4Music, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
				Once = 1;
			}
	}

	//player
	DrawEntity(Spaceship);

	DrawGrid(AlienGrid);

	for (int i = 0; i < 10; i++)
	{
		if (Bullets[i] != NULL)
		{
			DrawEntity(Bullets[i]);
		}
	}

	//Render GUI
	//Game Render Rectangle
	al_draw_rectangle(PlaySpacePos.x, PlaySpacePos.y, PlaySpacePos.x + PlaySpaceArea.x, PlaySpacePos.y + PlaySpaceArea.y, GUIColor, 0);
	//Top	
	al_draw_filled_rectangle(0, 0, ScreenDimensions.x, PlaySpacePos.y, GUIColor);
	//Left
	al_draw_filled_rectangle(0, PlaySpacePos.y, PlaySpacePos.x, PlaySpacePos.y + PlaySpaceArea.y, GUIColor);
	//Right
	al_draw_filled_rectangle(PlaySpacePos.x + PlaySpaceArea.x, PlaySpacePos.y, ScreenDimensions.x, PlaySpacePos.y + PlaySpaceArea.y, GUIColor);
	//Bottom
	al_draw_filled_rectangle(0, PlaySpacePos.y + PlaySpaceArea.y, ScreenDimensions.x, ScreenDimensions.y, GUIColor);

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