#include "Game.h"
#include "Entity.h"

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
#ifdef __linux__
	pc = false;
	raspi = true;
#elif _WIN64
	pc = true;
	raspi = false;
#endif
	int error = 0;

	al_init();

	al_init_image_addon();

	al_install_audio();
	al_init_acodec_addon();

	al_install_keyboard();
	al_install_mouse();

	al_init_primitives_addon();

	ScreenDimensions = NewVec2(1920, 1080);

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

	int	SpaceshipWidth = 120;
	int	SpaceshipHeight = 60;

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
	
	// Background Init
	background1 = al_load_bitmap(LVL1_BG);
	background2 = al_load_bitmap(LVL2_BG);
	background3 = al_load_bitmap(LVL3_BG);
	background4 = al_load_bitmap(LVL4_BG);
	backgroundpause = al_load_bitmap(PAUSE_BG);


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

	// Music Init
	level1Music = al_load_sample(MUSIC_LEVEL1);
	level2Music = al_load_sample(MUSIC_LEVEL2);
	level3Music = al_load_sample(MUSIC_LEVEL3);
	level4Music = al_load_sample(MUSIC_LEVEL4);

	Bullet_sound = al_load_sample(PLAYERSHOTSFX);
	alien_death_sound = al_load_sample(ALIENDEATHSFX);

	instance1 = al_create_sample_instance(PLAYERSHOTSFX);

	// Characters Init

	MiniUFO = NewSpriteSheet(MINIUFO1SP, (float)((float)1 / (float)17), 16, 44, 38, 1);
	Slug = NewSpriteSheet(SLUG, (float)((float)1 / (float)20), 20, 66, 38, 1);

	Spaceship = CreateNewAnimatedEntityLoadedTexture(NewVec2F(ScreenDimensions.x/2 - 50/2, PlaySpacePos.y + PlaySpaceArea.y - 80 ), NewVec2F(0, 0), Slug, SpaceshipWidth, SpaceshipHeight);
	if (Spaceship == NULL)
	{
		printf("There has been an error creating the player spaceship");
		error = -1;
	}

	Gun = CreateNewEntity(NewVec2F(0, PlaySpacePos.y + PlaySpaceArea.y - 130), NewVec2F(0, 0), GUN_TEXTURE, 90, 90);

	Bullets[0] = malloc(sizeof(Entity) * 10);
	if (Bullets != NULL)
	{
		for (int i = 0; i < 10; i++)
		{
			Bullets[i] = NULL;
		}
	}
	BulletTexture = al_load_bitmap(BULLET_TEXTURE1);
	DeathTexture = al_load_bitmap(DEATH_TEXTURE);

	MiniUFO = NewSpriteSheet(MINIUFO1SP, (float)((float)1 / (float)12), 16, 44, 38 , 1);

	return error;
}

void GameDestroy()
{
	for (int i = 0; i < 10; i++)
	{
		if (Bullets[i] != NULL)
		{
			free(Bullets[i]);
		}
	}

	DestroyMatrix(AlienGrid);
	DestroyAnimatedEntity(Spaceship);

	al_destroy_bitmap(menu);
	al_destroy_display(DISPLAY);
	al_destroy_user_event_source(KeyboardEventSource);
	al_destroy_user_event_source(MouseEventSource);
	al_destroy_event_queue(InputEventQueue);

	al_destroy_sample(background1);
	al_destroy_sample(background2);
	al_destroy_sample(background3);
	al_destroy_sample(background4);

	al_destroy_sample_instance(instance1);

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
	Pause();

}

void Pause()
{
	if (pause)
	{
		al_draw_scaled_bitmap(backgroundpause, 0, 0, al_get_bitmap_width(backgroundpause), al_get_bitmap_height(backgroundpause), PlaySpacePos.x, PlaySpacePos.y, PlaySpaceArea.x, PlaySpaceArea.y, NULL);
		al_flip_display();
	}
	while (pause)
	{
		if (!al_is_event_queue_empty(InputEventQueue))
		{
			al_get_next_event(InputEventQueue, &TempEvent);


			switch (TempEvent.type)
			{
			case ALLEGRO_EVENT_KEY_DOWN:
				switch (TempEvent.keyboard.keycode)
				{
				case ALLEGRO_KEY_ESCAPE:
					pause = 0;
				}

			}
		}
	}
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
				case ALLEGRO_KEY_ESCAPE:
					pause = 1;
					break;
				case ALLEGRO_KEY_A:
					Spaceship->Vel.x -= SHIP_SPEED;
					Gun->Vel.x -= SHIP_SPEED;
					break;
				case ALLEGRO_KEY_D:
					Spaceship->Vel.x += SHIP_SPEED;
					Gun->Vel.x += SHIP_SPEED;
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
					shot = 1;
					for (int i = 0; i < 10; i++)
					{
						if (i == 9)
						{
							Bullets[i] = CreateNewEntityLoadedTexture(NewVec2F((int)(Spaceship->Pos.x) + Spaceship->width / 2 - 24 / 2, Gun->Pos.y - 2), NewVec2F(0, -600), BulletTexture , al_get_bitmap_width(BulletTexture), al_get_bitmap_height(BulletTexture));
							break;
						}

						if (Bullets[i] == NULL)
						{
							Bullets[i] = CreateNewEntityLoadedTexture(NewVec2F((int)(Spaceship->Pos.x) + Spaceship->width / 2 - 24 / 2, Gun->Pos.y - 2), NewVec2F(0, -600), BulletTexture, al_get_bitmap_width(BulletTexture), al_get_bitmap_height(BulletTexture));
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
				Gun->Vel.x += SHIP_SPEED;
				break;
			case ALLEGRO_KEY_D:
				Spaceship->Vel.x -= SHIP_SPEED;
				Gun->Vel.x -= SHIP_SPEED;
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


	UpdateEntity(Spaceship, DeltaTime);
	UpdateEntity(Gun, DeltaTime);
	ClipToScreen(Spaceship, ScreenDimensions);
	ClipToEntity(Gun, Spaceship, 10);
	Animate(Spaceship, DeltaTime);

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
	switch ((Level - 1) % 4)
	{
	case 0:
		al_draw_scaled_bitmap(background1, 0, 0, al_get_bitmap_width(background1), al_get_bitmap_height(background1), PlaySpacePos.x, PlaySpacePos.y, PlaySpaceArea.x, PlaySpaceArea.y, NULL);
		break;
	case 1:
		al_draw_scaled_bitmap(background2, 0, 0, al_get_bitmap_width(background2), al_get_bitmap_height(background2), PlaySpacePos.x, PlaySpacePos.y, PlaySpaceArea.x, PlaySpaceArea.y, NULL);
		break;
	case 2:
		al_draw_scaled_bitmap(background3, 0, 0, al_get_bitmap_width(background3), al_get_bitmap_height(background3), PlaySpacePos.x, PlaySpacePos.y, PlaySpaceArea.x, PlaySpaceArea.y, NULL);
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
			al_play_sample(level3Music, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
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
	DrawEntity(Gun);
	DrawEntity(Spaceship);

	//Enemies

	aliendeath = CollideGrid(Bullets, AlienGrid, Deaths, DeathTexture);
	DrawGrid(AlienGrid);

	for (int h = 0; h < 20; h++)
	{
		if (Deaths[h] != NULL)
		{
			DrawEntity(Deaths[h]);
			if (Deaths[h] != NULL)
			{

//				Deaths[h]->frameCount++

//				if (Deaths[h]->deltaFrame >= Deaths[h]->spriteS->maxDeltaFrame)
//				{
//					DestroyEntityLoadedTexture(Deaths[h]);
//					Deaths[h] = NULL;
//
//				}
//				DestroyEntity(Deaths[h]);

			}
			
		}
	}


	//fight sounds
	if (shot)
	{
		al_play_sample(Bullet_sound, 0.3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
		shot = 0;
	}
	if (aliendeath)
	{
		al_play_sample(alien_death_sound, 0.3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
	}

	

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