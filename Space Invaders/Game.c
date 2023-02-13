#include "Game.h"

void Game()
{
	SystemInit();

	while (ESTADO != STOP)
	{
		switch (ESTADO)
		{
			case MENU:
				Menu();
				break;
			case GAME:
			{
				GameInit();
				while (GAMESTATE != EXIT)
				{
					GameLoop();
				}
				GameDestroy();
				break;
			}
		}
			
	}

	SystemDestroy();
	return;
}

int SystemInit()
{


	int error = 0;
#ifndef RASPI_TEST
	al_init();
	al_init_primitives_addon();
#endif

#ifndef RASPI
	al_init_image_addon();

	al_install_audio();
	al_init_acodec_addon();

	al_install_keyboard();
	al_install_mouse();


	al_init_font_addon();
	al_init_ttf_addon();

	InitAnimations();

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

	LightGrayOverlay = al_load_bitmap(LIGHTGRAYOVERLAY);
	BlackOverlay = al_load_bitmap(BLACKOVERLAY);

	al_reserve_samples(SAMPLE_COUNT);

	font = al_load_ttf_font(FONT, 36, NULL);
	BigFont = al_load_ttf_font(FONT, 80, NULL);
#endif
#ifdef RASPI
	InitGraphics();
	initInput();
	InitAudio();


	pickcooldown = 0.5 * TIME_MULTIPLIER;
	HasMovedAlready = 0;
#endif
	ESTADO = MENU;
}

int Menu()
{
#ifndef RASPI
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
					&& (MenuEvent.mouse.y < (ScreenDimensions.y - 50)) && (MenuEvent.mouse.y > (ScreenDimensions.y - 50 - height))))
				{
						ready = 1;
				}
			}			

		}

		al_flip_display();

	}
	al_stop_samples();
#endif
#ifdef RASPI
	ClearGrid();
	char PlayButtonShape[] = { 0,0,0,0,
							   1,0,0,0,
							   1,1,0,0,
							   1,1,1,0,
							   1,1,0,0,
							   1,0,0,0,
							   0,0,0,0 };
	PlayButton = CreateNewEntity(NewVec2(7, 1), NewVec2(0, 0), 2000, PlayButtonShape, NewVec2(4, 7));

	char ExitButtonShape[] = { 1,0,0,0,1,
							   0,1,0,1,0,
							   0,0,1,0,0,
							   0,1,0,1,0,
							   1,0,0,0,1 };
	ExitButton = CreateNewEntity(NewVec2(6, 9), NewVec2(0, 0), 2000, ExitButtonShape, NewVec2(5, 5));

	char PickCircleShape[] = {1,1,1,1,1,1,1,
							  1,0,0,0,0,0,1,
							  1,0,0,0,0,0,1,
							  1,0,0,0,0,0,1,
							  1,0,0,0,0,0,1,
							  1,0,0,0,0,0,1,
							  1,1,1,1,1,1,1 };
	PickCircle = CreateNewEntity(NewVec2(5, 1), NewVec2(0, 0), 2000, PickCircleShape, NewVec2(7, 7));

	char SmallPickCircleShape[] = { 1,1,1,1,1,1,
									1,0,0,0,0,1,
									1,0,0,0,0,1,
									1,0,0,0,0,1,
									1,0,0,0,0,1,
									1,1,1,1,1,1 };

	FacePickCircle = CreateNewEntity(NewVec2(5, 1), NewVec2(0, 0), 2000, SmallPickCircleShape, NewVec2(6, 6));

	int MENU_STATE;
	enum MENU_STATE { MAIN_MENU , DIFFICULTY_MENU , EXIT_GAME};

	MENU_STATE = MAIN_MENU;

	static double twinkleBuffer;
	double twinkleCoodlwon = 0.2 * TIME_MULTIPLIER;
	int showCircle = 1;

	int PICKER_STATE = PLAY;

	char EasyShape[] = { 0,0,0,0,0,0,
						 0,1,0,0,1,0,
						 0,0,0,0,0,0,
						 0,1,0,0,1,0,
						 0,0,1,1,0,0,
						 0,0,0,0,0,0 };

	char NormalShape[] = { 0,0,0,0,0,0,
						   0,1,0,0,1,0,
						   0,0,0,0,0,0,
						   0,0,0,0,0,0,
						   0,1,1,1,1,0,
						   0,0,0,0,0,0 };

	char HardShape[] = { 0,0,0,0,0,0,
						 0,1,0,0,1,0,
						 0,0,0,0,0,0,
						 0,0,1,1,0,0,
						 0,1,0,0,1,0,
						 0,0,0,0,0,0 };

	char HardcoreShape[] = { 0,0,0,0,0,0,
							 0,1,0,0,1,0,
							 0,1,0,1,1,0,
							 0,1,1,1,1,0,
							 0,1,1,0,1,0,
							 0,0,0,0,0,0 };

	EasyFace = CreateNewEntity(NewVec2(2, 2), NewVec2(0, 0), 2000, EasyShape, NewVec2(6, 6));
	NormalFace = CreateNewEntity(NewVec2(8, 2), NewVec2(0, 0), 2000, NormalShape, NewVec2(6, 6));
	HardFace = CreateNewEntity(NewVec2(2, 8), NewVec2(0, 0), 2000, HardShape, NewVec2(6, 6));
	HardcoreFace = CreateNewEntity(NewVec2(8, 8), NewVec2(0, 0), 2000, HardcoreShape, NewVec2(6, 6));


	while (ESTADO == MENU)
	{
		t = clock();
		ClearGrid();

		switch (MENU_STATE)
		{
		case MAIN_MENU:
		{
			lastMovement += DeltaTime;
			if (isPressingKey(SHOOT) && lastMovement > pickcooldown)
			{
				lastMovement = 0;
				switch (PICKER_STATE)
				{
				case PLAY:
					MENU_STATE = DIFFICULTY_MENU;
					PICKER_STATE = EASY;
					lastMovement = 0;
					break;
				case EXIT_MENU:
					MENU_STATE = EXIT_GAME;
					ESTADO = STOP;
					break;
				}

				PlayAudio(SHIELD_HIT_t);
					

			}
			
			if (isPressingKey(DOWN))
			{
				switch (PICKER_STATE)
				{
				case PLAY:
					PICKER_STATE = EXIT_MENU;
					break;
				case EXIT_MENU:
					break;
				}
			}
			if (isPressingKey(UP))
			{
				switch (PICKER_STATE)
				{
				case PLAY:
					break;
				case EXIT_MENU:
					PICKER_STATE = PLAY;
					break;
				}
			}

			switch (PICKER_STATE)
			{
			case PLAY:
				PickCircle->Pos = (Vec2){ 5, 1 };
				break;
			case EXIT_MENU:
				PickCircle->Pos = (Vec2){ 5,8 };
			}

			twinkleBuffer += DeltaTime;
			if (twinkleBuffer >= twinkleCoodlwon)
			{
				twinkleBuffer -= twinkleCoodlwon;
				showCircle *= -1;
			}
			if (showCircle == 1)
			{
				DrawEntity(PickCircle);
			}
			DrawEntity(PlayButton);
			DrawEntity(ExitButton);
			PrintGrid();
			t = clock() - t;
			DeltaTime = (double)(t) / (CLOCKS_PER_SEC);

		}
		break;
		case DIFFICULTY_MENU:

			lastMovement += DeltaTime;
			if (isPressingKey(UP) && !HasMovedAlready)
			{
				switch (PICKER_STATE)
				{
				case EASY:
				case NORMAL:
					break;
				case HARD:
					PICKER_STATE = EASY;
					break;
				case HARDCORE:
					PICKER_STATE = NORMAL;

				}
			}
			if (isPressingKey(DOWN) && !HasMovedAlready)
			{
				switch (PICKER_STATE)
				{
				case EASY:
					PICKER_STATE = HARD;
					break;
				case NORMAL:
					PICKER_STATE = HARDCORE;
					break;
				case HARD:
				case HARDCORE:
					break;
				}
			}
			if (isPressingKey(RIGHT) && !HasMovedAlready)
			{
				switch (PICKER_STATE)
				{
				case EASY:
					PICKER_STATE = NORMAL;
					break;
				case NORMAL:
					break;
				case HARD:
					PICKER_STATE = HARDCORE;
					break;
				case HARDCORE:
					break;
				}
			}
			if (isPressingKey(LEFT) && !HasMovedAlready)
			{
				switch (PICKER_STATE)
				{
				case EASY:
					break;
				case NORMAL:
					PICKER_STATE = EASY;
					break;
				case HARD:
					break;
				case HARDCORE:
					PICKER_STATE = HARD;
					break;
				}
			}

			switch (PICKER_STATE)
			{
			case EASY:
				FacePickCircle->Pos = EasyFace->Pos;
				break;
			case NORMAL:
				FacePickCircle->Pos = NormalFace->Pos;
				break;
			case HARD:
				FacePickCircle->Pos = HardFace->Pos;
				break;
			case HARDCORE:
				FacePickCircle->Pos = HardcoreFace->Pos;
				break;
			}

			ClearGrid();
			twinkleBuffer += DeltaTime;
			if (twinkleBuffer >= twinkleCoodlwon)
			{
				twinkleBuffer -= twinkleCoodlwon;
				showCircle *= -1;
			}
			if (showCircle == 1)
			{
				DrawEntity(FacePickCircle);
			}

			if (isPressingKey(SHOOT) && lastMovement > pickcooldown)
			{
				difficulty = PICKER_STATE;
				ESTADO = GAME;
				lastMovement = 0;
				PlayAudio(SHIELD_HIT_t);
			}


				
			DrawEntity(EasyFace);
			DrawEntity(NormalFace);
			DrawEntity(HardFace);
			DrawEntity(HardcoreFace);
			PrintGrid();

			t = clock() - t;
			DeltaTime = (double)(t) / (CLOCKS_PER_SEC);

		break;
		}

		

	}
	DestroyEntity(PlayButton);
	DestroyEntity(PickCircle);
	DestroyEntity(FacePickCircle);
	DestroyEntity(EasyFace);
	DestroyEntity(NormalFace);
	DestroyEntity(HardFace);
	DestroyEntity(HardcoreFace);
#endif
	if (ESTADO != STOP)
	{
		ESTADO = GAME;
	}
}

int GameInit()
{
#ifndef RASPI
	int error = 0;

	int AlienPaddingX = 20;
	int AlienPaddingY = 10;
	XAliens = 11;
	YAliens = 5;
	AlienWidth = 45;
	AlienHeight = 45;

	int	SpaceshipWidth = 180;
	int	SpaceshipHeight = 100;
	int GunWidth = 120;
	int GunHeight = 120;
	int SpaceshipYcoord = 950;
	int GunYcoord = 880;

	PlaySpaceArea = (Vec2){ 1000, ScreenDimensions.y };
	PlaySpacePos = NewVec2(ScreenDimensions.x / 2 - PlaySpaceArea.x / 2, 0);
	GUIColor = al_map_rgb(40, 60, 20);

	AlienTexture = al_load_bitmap(ALIEN_TEXTURE1);
	if (AlienTexture == NULL)
	{
		printf("There has been an error loading the alien texture");
		error = -1;
	}

	AlienGrid = NewMatrix(AlienPaddingX, AlienPaddingY, AlienWidth, AlienHeight, XAliens, YAliens, 15);
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
	pready = al_load_bitmap(PREADY);
	pmenu = al_load_bitmap(PMENU);

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
	BigUFO_sound = al_load_sample(BIGUFO_MUSIC);
	ok_sound = al_load_sample(OK_MUSIC);


	Bullet_sound = al_load_sample(PLAYERSHOTSFX);
	ShotNotReadySFX = al_load_sample(SHOTNOTREADY);
	ShipImpactSFX = al_load_sample(PLAYERIMPACTSFX);
	alien_death_sound = al_load_sample(ALIENDEATHSFX);

	ShieldImpact = al_load_sample(SHIELD_IMPACT);
	ShieldDestroyed = al_load_sample(SHIELD_EXPLOSION);

	BigExplosion = NewSpriteSheet(BIG_EXPLOSION, (float)((float)1 / (float)20), 28, 81, 94, 1);

	instance1 = al_create_sample_instance(PLAYERSHOTSFX);

	// Characters Init
	Slug = NewSpriteSheet(SLUG, (float)((float)1 / (float)20), 20, 66, 38, 1);
	Stp_b = NewSpriteSheet(STOP_BACKWARDS, (float)((float)1 / (float)20), 20, 66, 38, 1);
	Stp_f = NewSpriteSheet(STOP_FORWARDS, (float)((float)1 / (float)20), 20, 66, 38, 1);

	Spaceship = CreateNewAnimatedEntityLoadedTexture(NewVec2F(ScreenDimensions.x / 2 - 50 / 2, SpaceshipYcoord), NewVec2F(0, 0), Slug, SpaceshipWidth, SpaceshipHeight);
	Spaceship->data = 0; //El Spaceship esta quieto esperando movimiento

	if (Spaceship == NULL)
	{
		printf("There has been an error creating the player spaceship");
		error = -1;
	}

	Gun = CreateNewEntity(NewVec2F(0, GunYcoord), NewVec2F(0, 0), GUN_TEXTURE, GunWidth, GunHeight);

	Bullets[0] = malloc(sizeof(Entity) * 10);
	if (Bullets != NULL)
	{
		for (int i = 0; i < 10; i++)
		{
			Bullets[i] = NULL;
		}
	}
	BulletTexture = al_load_bitmap(BULLET_TEXTURE1);

	AlienBullets[0] = calloc(15, sizeof(Entity));
	AlienBullet = NewSpriteSheet(ALIENBULLETS, (float)((float)1 / (float)12), 3, 5, 32, 1);

	BulletExplosion = NewSpriteSheet(BULLET_EXPLOSION_SS, (float)((float)1 / (float)20), 14, 35, 35, 1);

	DeathTexture = al_load_bitmap(DEATH_TEXTURE);

	MiniUFO = NewSpriteSheet(MINIUFO1SP, (float)((float)1 / (float)12), 16, 44, 38, 1);

	MiniUFO_Explosion = NewSpriteSheet(EXPLOSION_SPRITE, (float)((float)1 / (float)15), 22, 70, 70, 1);

	BigUFO = NewSpriteSheet(BIGUFO1SP, (float)((float)1 / (float)12), 12, 56, 38, 1);

	ShieldExplosion = NewSpriteSheet(SHIELD_EXPLOSION_SS, (float)((float)1 / (float)12), 27, 70, 70, 1);

	//Miscellaneous assets

	heart = al_load_bitmap(HEART);
	deadheart = al_load_bitmap(DHEART);

//	BigUFOent = CreateNewAnimatedEntityLoadedTexture(NewVec2F(500, 500), NewVec2F(100, 0), BigUFO, 56, 38);

	numberOfShields = 3;
	shieldSize = NewVec2F(150, 70);
	shieldPadding = (PlaySpaceArea.x - shieldSize.x * numberOfShields) / (numberOfShields + 1);
	float shieldYpos = Spaceship->Pos.y - PlaySpaceArea.y * 0.15;

	shieldArray[0] = calloc(numberOfShields, sizeof(shield));

	ShieldTexture = al_load_bitmap(SHIELD);
	if (shieldArray != NULL)
	{
		for (int i = 0; i < numberOfShields; i++)
		{
			shieldArray[i] = CreateNewShield(NewVec2F(PlaySpacePos.x + shieldPadding + (i * (shieldSize.x + shieldPadding)), shieldYpos), NewVec2F(0, 0), NewVec2F(150, 70), 10, ShieldTexture);
			FillShieldParticles(shieldArray[i]);
		}
	}

	//Score system init

	Once = 0;
	lives = 3;
	ALLEGRO_EVENT TempEvent;
	while (!al_get_next_event(InputEventQueue , &TempEvent));

#endif
#ifdef RASPI
	ClearGrid();
	char AlienShape[] = { 1 , 1, 1,
						0 , 1 , 0 };
	AlienGrid = CreateNewAlienMatrix(NewVec2(2, 1), 1, 3, 2, NewVec2(3, 2), 1, AlienShape);
	FillMatrix(AlienGrid);

	char PlayerShape[] = { 0 , 1, 0,
						1 , 1 , 1 };
	Spaceship = CreateNewEntity(NewVec2(6, 13), NewVec2(0, 0), 0.1 * TIME_MULTIPLIER , PlayerShape, NewVec2(3, 2));

	BigUFOent = NULL;

	BulletShape[0] = ( 1 );

	for (int i = 0; i < MAX_BULLETS; i++)
	{
		Bullets[i] = NULL;
	}

	numberOfShields = 3;
	char shieldShape[] = { 1,1,1,1
						,1,0,0,1 };
	Vec2 shieldDimensions = NewVec2(4, 2);

	shieldArray[0] = calloc(numberOfShields, sizeof(Entity));

	for (int i = 0; i < numberOfShields; i++)
	{
		shieldArray[i] = CreateNewEntity(NewVec2(1 + i * (shieldDimensions.x + 1), 10), NewVec2(0, 0), 1, shieldShape, shieldDimensions);
	}

	switch (difficulty)
	{
	case EASY:
		AlienShootInterval = 5;
		AlienShotSpeed = 0.4;
		lives = 3;
		break;
	case NORMAL:
		AlienShootInterval = 2.5;
		AlienShotSpeed = 0.2;
		lives = 3;
		break;
	case HARD:
		AlienShootInterval = 1;
		AlienShotSpeed = 0.1;
		lives = 3;
		break;
	case HARDCORE:
		AlienShootInterval = 1;
		AlienShotSpeed = 0.1;
		lives = 1;
		break;
	}

#endif

	Level = 0;
	aliensDestroyed = 0;
	BigUFOsDestroyed = 0;
	score = 0;

	GAMESTATE = PLAYING_STATE;
	return 0;
}

void GameDestroy()
{
#ifndef RASPI

	for (int i = 0; i < MAX_ANIMATION_BUFFER_SIZE; i++)
	{
		if ((*AnimationBuffer)[i] != NULL)
		{
			DestroyAnimation((*AnimationBuffer)[i]);
			(*AnimationBuffer)[i] = NULL;
		}
	}

	for (int i = 0; i < numberOfShields; i++)
	{
		if (shieldArray[i] != NULL)
		{
			DestroyShield(shieldArray[i]);
			shieldArray[i] = NULL;
		}
	}

	al_destroy_bitmap(ShieldTexture);

	al_destroy_bitmap(heart);
	al_destroy_bitmap(deadheart);

	DeleteSpriteSheet(BulletExplosion);
	al_destroy_bitmap(DeathTexture);
	DeleteSpriteSheet(MiniUFO_Explosion);
	DeleteSpriteSheet(ShieldExplosion);

	DestroyEntity(Gun);
	DeleteSpriteSheet(MiniUFO);
	DeleteSpriteSheet(BigUFO);
	DeleteSpriteSheet(Slug);

	for (int i = 0; i < 10; i++)
	{
		if (Bullets[i] != NULL)
		{
			DestroyEntityLoadedTexture(Bullets[i]);
			Bullets[i] = NULL;
		}
	}

	for (int i = 0; i < 15; i++)
	{
		if (AlienBullets[i] != NULL)
		{
			DestroyAnimatedEntitySharedSprite(AlienBullets[i]);
			AlienBullets[i] = NULL;
		}
	}
	DeleteSpriteSheet(AlienBullet);
	al_destroy_bitmap(BulletTexture);

	DestroyMatrix(AlienGrid);
	DestroyAnimatedEntitySharedSprite(Spaceship);
	al_destroy_bitmap(AlienTexture);

	al_destroy_bitmap(background1);
	al_destroy_bitmap(background2);
	al_destroy_bitmap(background3);
	al_destroy_bitmap(background4);
	al_destroy_bitmap(backgroundpause);
	al_destroy_bitmap(pready);
	al_destroy_bitmap(pmenu);

	al_destroy_sample(level1Music);
	al_destroy_sample(level2Music);
	al_destroy_sample(level3Music);
	al_destroy_sample(level4Music);


	al_destroy_sample(Bullet_sound);
	al_destroy_sample(ShotNotReadySFX);
	al_destroy_sample(ShipImpactSFX);
	al_destroy_sample(alien_death_sound);
	al_destroy_sample(BigUFO_sound);
	al_destroy_sample(ok_sound);

	al_destroy_sample(ShieldImpact);
	al_destroy_sample(ShieldDestroyed);



	al_destroy_sample_instance(instance1);
#endif
	return;
}

void SystemDestroy()
{
#ifndef RASPI
	al_destroy_font(font);
	al_destroy_font(BigFont);
	al_destroy_bitmap(menu);
	al_destroy_display(DISPLAY);
	al_destroy_user_event_source(KeyboardEventSource);
	al_destroy_user_event_source(MouseEventSource);
	al_destroy_event_queue(InputEventQueue);
	al_shutdown_primitives_addon();
	al_uninstall_keyboard();
	al_uninstall_mouse();
	al_shutdown_image_addon();
#endif
#ifdef RASPI
	DestroyAudio();
#endif

	return;
}

void GameLoop()
{
	while (GAMESTATE != EXIT)
	{
		switch (GAMESTATE)
		{
			case PLAYING_STATE:
				Preframe();
				GameLogic();
				GameRender();
				Postframe();
				break;
			case PAUSE:
				Pause();
				break;
			case END:
				EndScreen();
				break;
		}
	}

}

void Pause()
{
#ifndef RASPI
	int scale = 3;
	al_draw_scaled_bitmap(backgroundpause, 0, 0, al_get_bitmap_width(backgroundpause), al_get_bitmap_height(backgroundpause), PlaySpacePos.x, PlaySpacePos.y, PlaySpaceArea.x, PlaySpaceArea.y, NULL);
	al_draw_scaled_bitmap(pready, 0, 0, al_get_bitmap_width(pready), al_get_bitmap_height(pready), (ScreenDimensions.x-(scale*al_get_bitmap_width(pready)))/2 + 40, (ScreenDimensions.y - (scale*al_get_bitmap_height(pready)))/ 2 - 150, scale*al_get_bitmap_width(pready), scale*al_get_bitmap_height(pready), NULL);
	al_draw_scaled_bitmap(pmenu, 0, 0, al_get_bitmap_width(pmenu), al_get_bitmap_height(pmenu), (ScreenDimensions.x - (scale * al_get_bitmap_width(pmenu))) / 2, (ScreenDimensions.y - (scale * al_get_bitmap_height(pmenu))) / 2 + 150, scale*al_get_bitmap_width(pmenu), scale*al_get_bitmap_height(pmenu), NULL);
	al_flip_display();
	int pause = true;
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
					GAMESTATE = PLAYING_STATE;
					break;
				}
			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
				if ((TempEvent.mouse.x < ((ScreenDimensions.x + (scale * al_get_bitmap_width(pready))) / 2 + 40 )) && (TempEvent.mouse.x > ((ScreenDimensions.x - (scale * al_get_bitmap_width(pready))) / 2 + 40)) && (TempEvent.mouse.y < (ScreenDimensions.y + (scale * al_get_bitmap_height(pready))) / 2 - 150) && (TempEvent.mouse.y > (ScreenDimensions.y - (scale * al_get_bitmap_height(pready))) / 2 - 150))
				{
					 pause = 0;
					 GAMESTATE = PLAYING_STATE;
				}
				if ((TempEvent.mouse.x < ((ScreenDimensions.x + (scale * al_get_bitmap_width(pmenu))) / 2)) && (TempEvent.mouse.x > ((ScreenDimensions.x - (scale * al_get_bitmap_width(pmenu))) / 2 + 40)) && (TempEvent.mouse.y < (ScreenDimensions.y + (scale * al_get_bitmap_height(pmenu))) / 2 + 150) && (TempEvent.mouse.y > (ScreenDimensions.y - (scale * al_get_bitmap_height(pmenu))) / 2 + 150))
				{
					pause = 0;
					GAMESTATE = EXIT;
					ESTADO = MENU;
				}
			}
		}
	}
#endif
#ifdef RASPI

	enum PICKER_STATE { RESUME, LEAVE };
	int PICKER_STATE = RESUME;

	char PickCircleShape[] = { 1,1,1,1,1,1,1,
							   1,0,0,0,0,0,1,
							   1,0,0,0,0,0,1,
							   1,0,0,0,0,0,1,
							   1,0,0,0,0,0,1,
							   1,0,0,0,0,0,1,
							   1,1,1,1,1,1,1 };
	PickCircle = CreateNewEntity(NewVec2(0, 0), NewVec2(0, 0), 2000, PickCircleShape, NewVec2(7, 7));

	char ResumeShape[] = { 1,0,0,0,
						   1,1,0,0,
						   1,1,1,0,
						   1,1,0,0,
						   1,0,0,0 };
	PlayButton = CreateNewEntity(NewVec2(3, 9), NewVec2(0, 0), 2000, ResumeShape, NewVec2(4, 5));

	char StopShape[] = { 0,0,0,0,
						 0,1,1,1,
						 0,1,1,1,
						 0,1,1,1,
						 0,0,0,0 };
	StopButton = CreateNewEntity(NewVec2(9, 9), NewVec2(0, 0), 2000, StopShape, NewVec2(4, 5));

	while (GAMESTATE == PAUSE)
	{
		ClearGrid();

		if (isPressingKey(RIGHT))
		{
			switch (PICKER_STATE)
			{
			case RESUME:
				PICKER_STATE = LEAVE;
				break;
			case LEAVE:
				break;
			}
		}
		if (isPressingKey(LEFT))
		{
			switch (PICKER_STATE)
			{
			case RESUME:
				break;
			case LEAVE:
				PICKER_STATE = RESUME;
				break;
			}
		}

		if (isPressingKey(SHOOT))
		{
			switch (PICKER_STATE)
			{
			case RESUME:
				GAMESTATE = PLAYING_STATE;
				break;
			case LEAVE:
				GAMESTATE = EXIT;
				ESTADO = MENU;
				lastMovement = 0;
			}
			PlayAudio(SHIELD_HIT_t);
		}

		switch (PICKER_STATE)
		{
		case RESUME:
			PickCircle->Pos = (Vec2){1,8};
			break;
		case LEAVE:
			PickCircle->Pos = (Vec2){ 8,8 };
			break;
		}

		DrawEntity(PlayButton);
		DrawEntity(StopButton);
		DrawEntity(PickCircle);

		// Print Score

		//if (score > 99)
		{
			int FirstDigit = score / 100;
			int SecondDigit =  ( score - FirstDigit  * 100 ) / 10;
			int ThirdDigit = score - FirstDigit * 100 - SecondDigit * 10;
			PrintNumber(FirstDigit, (Vec2) { 1, 1 });
			PrintNumber(SecondDigit, (Vec2) { 6, 1 });
			PrintNumber(ThirdDigit, (Vec2) { 11, 1 });
			
		}

		PrintGrid();
	}
#endif
	return;
}

void EndScreen()
{
#ifndef RASPI
	int end1 = true;
	int finished;
	int index;
	char a1[2] = {NULL, '\0'};
	char a2[2] = {NULL, '\0'};
	char a3[2] = {NULL, '\0'};

	al_stop_samples();

	while (GAMESTATE == END)
	{
		while (end1)
		{
			al_draw_scaled_bitmap(BlackOverlay, 0, 0, al_get_bitmap_width(BlackOverlay), al_get_bitmap_height(BlackOverlay), PlaySpacePos.x, PlaySpacePos.y, PlaySpaceArea.x, PlaySpaceArea.y, NULL);
			al_draw_text(BigFont, al_map_rgb(255, 255, 255), ScreenDimensions.x / 2, ScreenDimensions.y * 0.1, ALLEGRO_ALIGN_CENTER, "YOU DIED!");
			al_draw_text(font, al_map_rgb(255, 255, 255), ScreenDimensions.x / 2, ScreenDimensions.y * 0.1 * 3, ALLEGRO_ALIGN_CENTER, "YOUR SCORE:");
			char textscore[25];
			sprintf_s(textscore, 25, "%d", score * 10);
			al_draw_text(font, al_map_rgb(255, 255, 255), ScreenDimensions.x / 2, ScreenDimensions.y * 0.1 * 4, ALLEGRO_ALIGN_CENTER, textscore);
			al_draw_text(font, al_map_rgb(255, 255, 255), ScreenDimensions.x / 2, ScreenDimensions.y * 0.1 * 5, ALLEGRO_ALIGN_CENTER, "Invaders Vanquished:");
			char textInvadersKilled[25];
			sprintf_s(textInvadersKilled, 25, "%d", aliensDestroyed);
			al_draw_text(font, al_map_rgb(255, 255, 255), ScreenDimensions.x / 2, ScreenDimensions.y * 0.1 * 6, ALLEGRO_ALIGN_CENTER, textInvadersKilled);
			al_draw_text(font, al_map_rgb(255, 255, 255), ScreenDimensions.x / 2, ScreenDimensions.y * 0.1 * 7, ALLEGRO_ALIGN_CENTER, "Level Reached:");
			char textLevel[25];
			sprintf_s(textLevel, 25, "%d", Level);
			al_draw_text(font, al_map_rgb(255, 255, 255), ScreenDimensions.x / 2, ScreenDimensions.y * 0.1 * 8 - 25, ALLEGRO_ALIGN_CENTER, textLevel);
			al_draw_text(font, al_map_rgb(255, 255, 255), ScreenDimensions.x / 2, ScreenDimensions.y * 0.1 * 8 + 70, ALLEGRO_ALIGN_CENTER, "PRESS SPACE TO CONTINUE");

			al_flip_display();

			if (!al_is_event_queue_empty(InputEventQueue))
			{
				al_get_next_event(InputEventQueue, &TempEvent);


				switch (TempEvent.type)
				{
				case ALLEGRO_EVENT_KEY_DOWN:
					switch (TempEvent.keyboard.keycode)
					{
					case ALLEGRO_KEY_SPACE:
						end1 = 0;
						index = 0;
						finished = 0;
					}

				}
			}
		}

		if (!al_is_event_queue_empty(InputEventQueue))
		{
			al_get_next_event(InputEventQueue, &TempEvent);

			switch (TempEvent.type)
			{
			case ALLEGRO_EVENT_KEY_DOWN:
				switch (TempEvent.keyboard.keycode)
				{
				case ALLEGRO_KEY_SPACE:
					GAMESTATE = EXIT;
					ESTADO = MENU;
					break;
				case ALLEGRO_KEY_ENTER:
					finished = 1;
					al_play_sample(ok_sound, 0.5, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
			}
			if (!finished && (TempEvent.type != ALLEGRO_EVENT_MOUSE_AXES) && (TempEvent.type != ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY) && (TempEvent.type != ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) && (TempEvent.type != ALLEGRO_EVENT_KEY_UP))
			{
				switch (index % 3)
				{
				case 0:
					a1[0] = TempEvent.keyboard.keycode + 64;
					break;
				case 1:
					a2[0] = TempEvent.keyboard.keycode + 64;
					break;
				case 2:
					a3[0] = TempEvent.keyboard.keycode + 64;
					break;
				}
			}
			if ((TempEvent.type == ALLEGRO_EVENT_KEY_UP) && (TempEvent.keyboard.keycode != ALLEGRO_KEY_SPACE))
			{
				index++;
			}
			
		}

		al_draw_scaled_bitmap(BlackOverlay, 0, 0, al_get_bitmap_width(BlackOverlay), al_get_bitmap_height(BlackOverlay), PlaySpacePos.x, PlaySpacePos.y, PlaySpaceArea.x, PlaySpaceArea.y, NULL);
		al_draw_text(font, al_map_rgb(255, 255, 255), ScreenDimensions.x / 2, ScreenDimensions.y * 0.1 * 3, ALLEGRO_ALIGN_CENTER, "PLEASE ENTER YOUR NAME");
		al_draw_text(BigFont, al_map_rgb(255, 255, 255), ScreenDimensions.x / 2 - 150, ScreenDimensions.y * 0.1 * 3 + 180, ALLEGRO_ALIGN_CENTER, a1);
		al_draw_text(BigFont, al_map_rgb(255, 255, 255), ScreenDimensions.x / 2, ScreenDimensions.y * 0.1 * 3 + 180, ALLEGRO_ALIGN_CENTER, a2);
		al_draw_text(BigFont, al_map_rgb(255, 255, 255), ScreenDimensions.x / 2 + 150, ScreenDimensions.y * 0.1 * 3 + 180, ALLEGRO_ALIGN_CENTER, a3);
		al_draw_text(font, al_map_rgb(255, 255, 255), ScreenDimensions.x / 2, ScreenDimensions.y * 0.1 * 3 +340, ALLEGRO_ALIGN_CENTER, "PRESS ENTER");
		al_draw_text(font, al_map_rgb(255, 255, 255), ScreenDimensions.x / 2, ScreenDimensions.y * 0.1 * 3 + 420, ALLEGRO_ALIGN_CENTER, "WHEN YOU ARE DONE");
		al_draw_text(font, al_map_rgb(255, 255, 255), ScreenDimensions.x / 2, ScreenDimensions.y * 0.1 * 8 + 50, ALLEGRO_ALIGN_CENTER, "PRESS SPACE TO CONTINUE");
		al_flip_display();

	}

#endif

#ifdef RASPI
	enum PICKER_STATE { RETURN_MENU, LEAVE_GAME};
	int PICKER_STATE = RETURN_MENU;

	char PickCircleShape[] = { 1,1,1,1,1,1,1,
							   1,0,0,0,0,0,1,
							   1,0,0,0,0,0,1,
							   1,0,0,0,0,0,1,
							   1,0,0,0,0,0,1,
							   1,0,0,0,0,0,1,
							   1,1,1,1,1,1,1 };
	PickCircle = CreateNewEntity(NewVec2(0, 0), NewVec2(0, 0), 2000, PickCircleShape, NewVec2(7, 7));

	char ResumeShape[] = { 0,0,1,0,
						   0,1,1,0,
						   1,1,1,0,
						   0,1,1,0,
						   0,0,1,0 };
	PlayButton = CreateNewEntity(NewVec2(3, 10), NewVec2(0, 0), 2000, ResumeShape, NewVec2(4, 5));

	char StopShape[] = { 0,0,0,0,0,
						 0,1,0,1,0,
						 0,0,1,0,0,
						 0,1,0,1,0,
						 0,0,0,0,0 };
	StopButton = CreateNewEntity(NewVec2(9, 10), NewVec2(0, 0), 2000, StopShape, NewVec2(5, 5));

	char KOShape[] = { 0,1,0,1,0,1,1,1,0,1,
					   0,1,1,0,0,1,0,1,0,1,
					   0,1,0,1,0,1,1,1,0,0,
					   0,0,0,0,0,0,0,0,0,1};

	KOText = CreateNewEntity((Vec2) { 4, 0 }, (Vec2) { 0, 0 }, 2000, KOShape, (Vec2) { 10, 4 });

	while (GAMESTATE == END)
	{
		ClearGrid();

		if (isPressingKey(RIGHT))
		{
			switch (PICKER_STATE)
			{
			case RETURN_MENU:
				PICKER_STATE = LEAVE_GAME;
				break;
			case LEAVE_GAME:
				break;
			}
		}
		if (isPressingKey(LEFT))
		{
			switch (PICKER_STATE)
			{
			case RETURN_MENU:
				break;
			case LEAVE_GAME:
				PICKER_STATE = RETURN_MENU;
				break;
			}
		}

		if (isPressingKey(SHOOT))
		{
			switch (PICKER_STATE)
			{
			case RETURN_MENU:
				GAMESTATE = EXIT;
				ESTADO = MENU;
				break;
			case LEAVE_GAME:
				GAMESTATE = EXIT;
				ESTADO = STOP;
				lastMovement = 0;
			}
			PlayAudio(SHIELD_HIT_t);
		}

		switch (PICKER_STATE)
		{
		case RETURN_MENU:
			PickCircle->Pos = (Vec2){ 1,9 };
			break;
		case LEAVE_GAME:
			PickCircle->Pos = (Vec2){ 8,9 };
			break;
		}

		DrawEntity(PlayButton);
		DrawEntity(StopButton);
		DrawEntity(PickCircle);
		DrawEntity(KOText);

		
		{
			int FirstDigit = score / 100;
			int SecondDigit = (score - FirstDigit * 100) / 10;
			int ThirdDigit = score - FirstDigit * 100 - SecondDigit * 10;
			PrintNumber(FirstDigit, (Vec2) { 1, 4 });
			PrintNumber(SecondDigit, (Vec2) { 6, 4 });
			PrintNumber(ThirdDigit, (Vec2) { 11, 4 });

		}

		PrintGrid();
	}
#endif

}

void GameLogic()
{
	double shootCooldown;
	double MotherShip_cooldown = 40;
	static double shootElapsedTime = 0;
#ifndef RASPI

	static double Cooldown = 0;
	int fire_ready = 0;
	static int once = 1;
	static double Mothership_time = 0;

	switch (Level)
	{
	case 1: 
		Cooldown_c = 0.1;
		fire_ready = 1; 
	case 2:
		Cooldown_c = 0.3;
	case 3:
		Cooldown_c = 0.4;
	default:
		break;
	}

	if ((clock()/ CLOCKS_PER_SEC) > Cooldown)
	{
		fire_ready = 1;
	}
	if ((clock() / CLOCKS_PER_SEC) > Mothership_time)
	{
		Mothership_time = (clock() / CLOCKS_PER_SEC) + MotherShip_cooldown;
		BigUFOent = CreateNewAnimatedEntityLoadedTexture(NewVec2F(430, 50), NewVec2F(200, 0), BigUFO, 56, 38);
		BigUfo_passing = 1;
//		i = CreateNewAnimation(NewVec2F(500, 500), NewVec2F(100, 0), 10, BigUFO, 56, 38);
	}



	if (!al_is_event_queue_empty(InputEventQueue))
	{
		al_get_next_event(InputEventQueue, &TempEvent);

		switch (TempEvent.type)
		{
		case ALLEGRO_EVENT_KEY_DOWN:
			switch (TempEvent.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				GAMESTATE = PAUSE;
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
				lives -= 1;
				Once = 0;
				break;
			case ALLEGRO_KEY_F4:
				if (al_key_down(&KeyboardCurrentState, ALLEGRO_KEY_ALT))
				{
					GAMESTATE = EXIT;
					ESTADO = STOP;
				}
				break;
			case ALLEGRO_KEY_SPACE:

				if (fire_ready)
				{
					Cooldown = clock() / CLOCKS_PER_SEC + Cooldown_c;
					fire_ready = 0;
					shot = true;
					for (int i = 0; i < 10; i++)
					{
						if (i == 9)
						{
							Bullets[i] = CreateNewEntityLoadedTexture(NewVec2F((int)(Spaceship->Pos.x) + Spaceship->width / 2 - 24 / 2, Gun->Pos.y - 2), NewVec2F(0, -900), BulletTexture, al_get_bitmap_width(BulletTexture), al_get_bitmap_height(BulletTexture));
							break;
						}

						if (Bullets[i] == NULL)
						{
							Bullets[i] = CreateNewEntityLoadedTexture(NewVec2F((int)(Spaceship->Pos.x) + Spaceship->width / 2 - 24 / 2, Gun->Pos.y - 2), NewVec2F(0, -900), BulletTexture, al_get_bitmap_width(BulletTexture), al_get_bitmap_height(BulletTexture));
							break;
						}

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
	AnimateMatrix(AlienGrid, DeltaTime);

	CullBullets();
	UpdateBullets();
	UpdateAnimations(DeltaTime);

	ComputeAlienShot();

	UpdateEntity(Spaceship, DeltaTime);
	UpdateEntity(Gun, DeltaTime);
	aliendeath = CollideGrid(Bullets, AlienGrid, &aliensDestroyed, MiniUFO_Explosion);
	UpdateEntity(BigUFOent, DeltaTime);
	Animate(BigUFOent, DeltaTime);


	for (int i = 0; i < 10; i++)
	{
		if (Bullets[i] != NULL && BigUFOent != NULL)
		{
			if (AreColiding(BigUFOent, Bullets[i]))
			{
				CreateNewAnimation(BigUFOent->Pos, NewVec2F(0, 0), 0, BigExplosion, 79, 94);

				DestroyEntityLoadedTexture(Bullets[i]);
				Bullets[i] = NULL;
				
				DestroyAnimatedEntitySharedSprite(BigUFOent);
				BigUFOent = NULL;

				BigUFOsDestroyed++;

				return true;
			}
		}
	}

	score = aliensDestroyed * 10 + (Level - 1) * 100 + BigUFOsDestroyed * 100;

	ClipToScreen(Spaceship, ScreenDimensions);
	ClipToEntity(Gun, Spaceship, 20);

	AnimateBullets();
	CollideAlienBullets();

	if (Spaceship->Vel.x)
	{
	Animate(Spaceship, DeltaTime);
	}
	AlienBulletsHit();
	ProcessHP();

	if (AlienGrid->AlienCount == 0)
	{
		Level += 1;	//Pasa al siguiente nivel
		Mothership_time = (t / CLOCKS_PER_SEC) + MotherShip_cooldown;
		Once = 0;	//Resetea para el sonido

		for (int i = 0; i < 10; i++) //Limpia las balas que quedan volando cuando se quedan sin aliens
		{
			if (Bullets[i] != NULL)
			{
				DestroyEntityLoadedTexture(Bullets[i]);
				Bullets[i] = NULL;
			}
		}
		for (int i = 0; i < 15; i++)
		{
			if (AlienBullets[i] != NULL)
			{
				DestroyAnimatedEntitySharedSprite(AlienBullets[i]);
				AlienBullets[i] = NULL;
			}
		}
		for (int i = 1; i < MAX_ANIMATION_BUFFER_SIZE; i++)
		{
			if ((*AnimationBuffer)[i] != NULL)
			{
				DestroyAnimation((*AnimationBuffer)[i]);
			}
		}
		AlienGrid->Pos = GetCentredPosition(AlienGrid, ScreenDimensions);
		AlienGrid->Pos.y += 60;
		//FillMatrix(AlienGrid, AlienTexture);
		FillMatrixAnimated(AlienGrid, MiniUFO);


	}
#endif
#ifdef RASPI
	UpdateMatrixDynamic( AlienGrid,DeltaTime,NewVec2(0,0) , NewVec2(0, 0));
	UpdateEntity(Spaceship , DeltaTime);

	shootCooldown = 1 * TIME_MULTIPLIER;
	shootElapsedTime += DeltaTime;
	if (isPressingKey(LEFT))
	{
		Spaceship->Vel = NewVec2(-1, 0);
	}
	else if (isPressingKey(RIGHT))
	{
		Spaceship->Vel = NewVec2(1, 0);
	}
	else
	{
		Spaceship->Vel = NewVec2(0, 0);
	}

	if (isPressingKey(SHOOT))
	{
		if (shootElapsedTime >= shootCooldown)
		{
			shootElapsedTime = 0;
			for (int i = 1; i < MAX_BULLETS; i++)
			{
				if (Bullets[i] == NULL)
				{
					Bullets[i] = CreateNewEntity(NewVec2(Spaceship->Pos.x + Spaceship->dimensions.x / 2, Spaceship->Pos.y), NewVec2(0, -1), 0.1  * TIME_MULTIPLIER , BulletShape, NewVec2(1, 1));
					i = MAX_BULLETS;

					PlayAudio(SHOOT_t);
				}
			}
		}
		
	}

	if (isPressingKey(PAUSE_KEY))
	{
		GAMESTATE = PAUSE;
	}
	UpdateBullets();
	CullBullets();
	if (CollideGrid(Bullets, AlienGrid, &aliensDestroyed))
	{
		PlayAudio(INVADER_DEATH_t);
	}
	ClamToScreen(Spaceship);
	MotherShip();
	ComputeAlienShot();
	if (AlienBulletsHit())
	{
		PlayAudio(PLAYER_HIT_t);
	}
	ProcessHP();
	if (ColideAlienBullets())
	{
		PlayAudio(SHIELD_HIT_t);
	}

	if (AlienGrid->AlienCount <= 0)
	{
		Level++;
		for (int i = 0; i < MAX_BULLETS; i++)
		{
			if (Bullets[i] != NULL)
			{
				DestroyEntity(Bullets[i]);
				Bullets[i] = NULL;
			}
		}
		for (int i = 0; i < MAX_ALIEN_BULLETS; i++)
		{
			if (AlienBullets[i] != NULL)
			{
				DestroyEntity(AlienBullets[i]);
				AlienBullets[i] = NULL;
			}
		}
		AlienGrid->Pos = NewVec2(2, 1);
		FillMatrix(AlienGrid);
	}

	score = aliensDestroyed + (5 * Level) + (5 * BigUFOsDestroyed);
	static int ReplenishShields = 0;
	if (Level % 3 == 0 && ReplenishShields)
	{
		ReplenishShields = 0;
		char shieldShape[] = { 1,1,1,1
							  ,1,0,0,1 };
		Vec2 shieldDimensions = NewVec2(4, 2);

		for (int i = 0; i < numberOfShields; i++)
		{
			if (shieldArray != NULL)
			{
				DestroyEntity(shieldArray[i]);
				shieldArray[i] = CreateNewEntity(NewVec2(1 + i * (shieldDimensions.x + 1), 10), NewVec2(0, 0), 1, shieldShape, shieldDimensions);
			}
		}
	}
	else if (Level % 3 != 0)
	{
		ReplenishShields = 1;
	}

#endif

	return;
}

void GameRender()
{
#ifndef RASPI
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
			al_play_sample(level1Music, 0.4, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
			Once = 1;
		}
	case 1:
		if (Once == 0)
		{
			al_stop_samples();
			al_play_sample(level2Music, 0.4, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
			Once = 1;
		}
	case 2:
		if (Once == 0)
		{
			al_stop_samples();
			al_play_sample(level3Music, 0.4, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
			Once = 1;
		}
	case 3:
		if (Once == 0)
		{
			al_stop_samples();
			al_play_sample(level4Music, 0.4, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
			Once = 1;
		}
	}

	//fight sounds
	if (shot)
	{
		al_play_sample(Bullet_sound, 0.1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
		shot = 0;
	}
	if (aliendeath)
	{
		al_play_sample(alien_death_sound, 0.1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
	}
	if (shotOnCooldown)
	{
		al_play_sample(ShotNotReadySFX, 0.1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
		shotOnCooldown = false;
	}

	if (BigUfo_passing)
	{
		BigUfo_passing = 0;
		al_play_sample(BigUFO_sound, 0.2, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, &BigUFO_sound_id);
	}

	//player
	DrawEntity(Gun);
	DrawEntity(Spaceship);
	
	DrawEntity(BigUFOent);

	//Enemies

	
	DrawGrid(AlienGrid);

	for (int i = 0; i < numberOfShields; i++)
	{
		if (shieldArray[i] != NULL)
		{
			DrawShieldPartitions(shieldArray[i]);

		}
	}

	for (int i = 0; i < 10; i++)
	{
		if (Bullets[i] != NULL)
		{
			DrawEntity(Bullets[i]);
		}
	}

	for (int i = 0; i < 15; i++)
	{
		if (AlienBullets[i] != NULL)
		{
			DrawEntity(AlienBullets[i]);
		}
	}

	//Animations
	DrawAnimations();

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

	


	//Score

const char Sarray[10][3] = {"0\0", "1\0", "2\0", "3\0", "4\0", "5\0", "6\0", "7\0", "8\0", "9\0"};

	al_draw_text(font, al_map_rgb(254, 254, 254), 230, 60, ALLEGRO_ALIGN_CENTRE, "PLAYER SCORE");
	al_draw_text(font, al_map_rgb(254, 254, 254), 1580, 60, ALLEGRO_ALIGN_CENTRE, "LIVES");
	//corazoncito 
	
	if (lives > 0)
	{
		al_draw_scaled_bitmap(heart, 0, 0, al_get_bitmap_width(heart), al_get_bitmap_height(heart), 1690, 57, 55, 55, NULL);
	}
	else
	{
		al_draw_scaled_bitmap(deadheart, 0, 0, al_get_bitmap_width(deadheart), al_get_bitmap_height(deadheart), 1690, 57, 55, 55, NULL);
	}
	if (lives > 1)
	{
		al_draw_scaled_bitmap(heart, 0, 0, al_get_bitmap_width(heart), al_get_bitmap_height(heart), 1760, 57, 55, 55, NULL);
	}
	else
	{
		al_draw_scaled_bitmap(deadheart, 0, 0, al_get_bitmap_width(deadheart), al_get_bitmap_height(deadheart), 1760, 57, 55, 55, NULL);
	}
	if (lives > 2)
	{
		al_draw_scaled_bitmap(heart, 0, 0, al_get_bitmap_width(heart), al_get_bitmap_height(heart), 1830, 57, 55, 55, NULL);
	}
	else
	{
		al_draw_scaled_bitmap(deadheart, 0, 0, al_get_bitmap_width(deadheart), al_get_bitmap_height(deadheart), 1830, 57, 55, 55, NULL);
	}




			// Dato importante: El espacio entre cada letra es 40 pixeles
	al_draw_text(font, al_map_rgb(254, 254, 254), 370, 115, ALLEGRO_ALIGN_RIGHT, "0");
	al_draw_text(font, al_map_rgb(254, 254, 254), 330, 115, ALLEGRO_ALIGN_RIGHT, Sarray[(score%10)]);
	al_draw_text(font, al_map_rgb(254, 254, 254), 290, 115, ALLEGRO_ALIGN_RIGHT, Sarray[(score/10)%10]);
	al_draw_text(font, al_map_rgb(254, 254, 254), 250, 115, ALLEGRO_ALIGN_RIGHT, Sarray[(score/100)%10]);
	al_draw_text(font, al_map_rgb(254, 254, 254), 210, 115, ALLEGRO_ALIGN_RIGHT, Sarray[(score/1000) % 10]);
	al_draw_text(font, al_map_rgb(254, 254, 254), 170, 115, ALLEGRO_ALIGN_RIGHT, Sarray[(score/10000) % 10]);
	al_draw_text(font, al_map_rgb(254, 254, 254), 130, 115, ALLEGRO_ALIGN_RIGHT, Sarray[(score/100000) % 10]);

	al_draw_text(font, al_map_rgb(254, 254, 254), ( PlaySpacePos.x ) / 2 , 250, ALLEGRO_ALIGN_CENTRE, "TAKEDOWNS:");
	al_draw_textf(font, al_map_rgb(254, 254, 254), (PlaySpacePos.x) / 2, 300, ALLEGRO_ALIGN_CENTRE, "%d" , aliensDestroyed);

	al_draw_text(font, al_map_rgb(254, 254, 254), (PlaySpacePos.x) / 2, 400, ALLEGRO_ALIGN_CENTRE, "MOTHERSHIPS:");
	al_draw_textf(font, al_map_rgb(254, 254, 254), (PlaySpacePos.x) / 2, 450, ALLEGRO_ALIGN_CENTRE, "%d", BigUFOsDestroyed);

	al_draw_text(font, al_map_rgb(254, 254, 254), (PlaySpacePos.x) / 2, 550, ALLEGRO_ALIGN_CENTRE, "LEVEL:");
	al_draw_textf(font, al_map_rgb(254, 254, 254), (PlaySpacePos.x) / 2, 600, ALLEGRO_ALIGN_CENTRE, "%d", Level);


	al_flip_display();
#endif
#ifdef RASPI
	ClearGrid();
	
	DrawGrid(AlienGrid);
	DrawEntity(Spaceship);
	if (BigUFOent != NULL)
	{
		DrawEntity(BigUFOent);
	}
	DrawBullets();
	for (int i = 0; i < numberOfShields; i++)
	{
		if (shieldArray[i] != NULL)
		{
			DrawEntity(shieldArray[i]);
		}
	}

	if (lives > 2)
	{
		TurnOn(13, 15);
		
	}
	if (lives > 1)
	{
		TurnOn(14, 15);
	}
	if (lives > 0)
	{
		TurnOn(15, 15);
	}


	PrintGrid();
#endif
	return;
}

void Preframe()
{
#ifndef RASPI
	al_get_keyboard_state(&KeyboardCurrentState);
#endif
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

#ifndef RASPI
void ComputeAlienShot()
{
	static float timeBuffer;

	timeBuffer += DeltaTime;

	if (timeBuffer >= BASE_ALIEN_SHOT_SPEED)
	{
		timeBuffer = 0;

		float minimumDistanceToPlayer = 9999;
		float TempDistanceToPlayer;
		int closestAlienColumnToPlayer = (int)( AlienGrid->XAliens / 2);
		int lastAlienRow = 0;

		for (int i = 0; i < AlienGrid->XAliens; i++)
		{
			for (int j = (AlienGrid->YAliens - 1) ; j >= 0; j--)
			{
				if (AlienGrid->matrix[i][j] != NULL)
				{
					TempDistanceToPlayer = AbsoluteValue(AlienGrid->matrix[i][j]->Pos.x + (AlienGrid->matrix[i][j]->width / 2) - (Spaceship->Pos.x + (Spaceship->width / 2)));

					if (TempDistanceToPlayer < minimumDistanceToPlayer)
					{
						minimumDistanceToPlayer = TempDistanceToPlayer;
						closestAlienColumnToPlayer = i;
						lastAlienRow = j;
					}
					j = -1;
				}
			}
		}

		for (int i = 1; i < 15; i++)
		{
			if (AlienBullets[i] == NULL)
			{
				if (AlienGrid->matrix[closestAlienColumnToPlayer][lastAlienRow] != NULL)
				{
					AlienBullets[i] = CreateNewAnimatedEntityLoadedTexture(NewVec2F(AlienGrid->matrix[closestAlienColumnToPlayer][lastAlienRow]->Pos.x + AlienGrid->matrix[closestAlienColumnToPlayer][lastAlienRow]->width/2 , AlienGrid->matrix[closestAlienColumnToPlayer][lastAlienRow]->Pos.y + AlienGrid->matrix[closestAlienColumnToPlayer][lastAlienRow]->height)
						, NewVec2F(0, 400), AlienBullet, 5, 32);
					break;
				}
				
			}
		}
	}

}

void AnimateBullets()
{
	for (int i = 1; i < 15; i++)
	{
		if (AlienBullets[i] != NULL)
		{
			Animate(AlienBullets[i], DeltaTime);
		}
	}
}

void CollideAlienBullets()
{
	int shieldCollidedWith;
	for (int i = 1; i < 15; i++)
	{
		if (AlienBullets[i] != NULL)
		{
			if (AlienBullets[i]->data != 15)
			{
				for (int j = 0; j < numberOfShields; j++)
				{
					if (shieldArray[j] != NULL)
					{
						AlienBullets[i]->data = 15;
						if ((AlienBullets[i]->Pos.x >= shieldArray[j]->pos.x && AlienBullets[i]->Pos.x <= (shieldArray[j]->pos.x + shieldArray[j]->dimensions.x)) ||
							((AlienBullets[i]->Pos.x + AlienBullets[i]->width) >= shieldArray[j]->pos.x && (AlienBullets[i]->Pos.x + AlienBullets[i]->width) <= (shieldArray[j]->pos.x + shieldArray[j]->dimensions.x)))
						{
							AlienBullets[i]->data = j + 1;
							break;
						}
					}
				}
				if (AlienBullets[i]->data != 15 && shieldArray[AlienBullets[i]->data - 1] != NULL)
				{
					if (shieldArray[AlienBullets[i]->data - 1]->destroyed == false)
					{
						if ((AlienBullets[i]->Pos.y >= shieldArray[AlienBullets[i]->data - 1]->pos.y) &&
							(AlienBullets[i]->Pos.y - AlienBullets[i]->height <= shieldArray[AlienBullets[i]->data - 1]->pos.y + shieldArray[AlienBullets[i]->data - 1]->dimensions.y))
						{
							float hitShieldAt = (AlienBullets[i]->Pos.x + AlienBullets[i]->width) - shieldArray[AlienBullets[i]->data - 1]->pos.x;
							int divisionNumer = IMaxClampTo((hitShieldAt / shieldArray[AlienBullets[i]->data - 1]->Factors.x) / shieldArray[AlienBullets[i]->data - 1]->OXdelta, 29);
#ifdef _DEBUG
							al_draw_filled_circle(AlienBullets[i]->Pos.x, AlienBullets[i]->Pos.y, 20, al_map_rgb(255, 0, 0));
							al_flip_display();
#endif
							for (int l = 0; l < shieldArray[AlienBullets[i]->data - 1]->Ydivisions; l++)
							{
								if (shieldArray[AlienBullets[i]->data - 1]->Particles[divisionNumer][l] == true)
								{
									int OriginalX = divisionNumer;
									int OriginalY = l;
									shieldArray[AlienBullets[i]->data - 1]->Particles[divisionNumer][IMaxClampTo(l++, shieldArray[AlienBullets[i]->data - 1]->Ydivisions - 1)] = false;
									shieldArray[AlienBullets[i]->data - 1]->Particles[divisionNumer][IMaxClampTo(l++, shieldArray[AlienBullets[i]->data - 1]->Ydivisions - 1)] = false;
									shieldArray[AlienBullets[i]->data - 1]->Particles[divisionNumer][IMaxClampTo(l++, shieldArray[AlienBullets[i]->data - 1]->Ydivisions - 1)] = false;
									shieldArray[AlienBullets[i]->data - 1]->Particles[divisionNumer][IMaxClampTo(l++, shieldArray[AlienBullets[i]->data - 1]->Ydivisions - 1)] = false;
									shieldArray[AlienBullets[i]->data - 1]->Particles[divisionNumer][IMaxClampTo(l++, shieldArray[AlienBullets[i]->data - 1]->Ydivisions - 1)] = false;

									shieldArray[AlienBullets[i]->data - 1]->Particles[divisionNumer][IMaxClampTo(l + 2, shieldArray[AlienBullets[i]->data - 1]->Ydivisions - 1)] = false;
									shieldArray[AlienBullets[i]->data - 1]->Particles[divisionNumer][IMaxClampTo(l + 4, shieldArray[AlienBullets[i]->data - 1]->Ydivisions - 1)] = false;


									l = OriginalY;
									shieldArray[AlienBullets[i]->data - 1]->Particles[IMinClampTo(divisionNumer--, 0)][IMinClampTo(--l, 0)] = false;
									shieldArray[AlienBullets[i]->data - 1]->Particles[IMinClampTo(divisionNumer, 0)][IMaxClampTo(l++, shieldArray[AlienBullets[i]->data - 1]->Ydivisions - 1)] = false;
									shieldArray[AlienBullets[i]->data - 1]->Particles[IMinClampTo(divisionNumer, 0)][IMaxClampTo(l++, shieldArray[AlienBullets[i]->data - 1]->Ydivisions - 1)] = false;
									shieldArray[AlienBullets[i]->data - 1]->Particles[IMinClampTo(divisionNumer, 0)][IMaxClampTo(l++, shieldArray[AlienBullets[i]->data - 1]->Ydivisions - 1)] = false;
									shieldArray[AlienBullets[i]->data - 1]->Particles[IMinClampTo(divisionNumer, 0)][IMaxClampTo(l++, shieldArray[AlienBullets[i]->data - 1]->Ydivisions - 1)] = false;
									shieldArray[AlienBullets[i]->data - 1]->Particles[IMinClampTo(divisionNumer, 0)][IMaxClampTo(l++, shieldArray[AlienBullets[i]->data - 1]->Ydivisions - 1)] = false;
									shieldArray[AlienBullets[i]->data - 1]->Particles[IMinClampTo(divisionNumer, 0)][IMaxClampTo(l + 2, shieldArray[AlienBullets[i]->data - 1]->Ydivisions - 1)] = false;

									l = OriginalY;
									shieldArray[AlienBullets[i]->data - 1]->Particles[IMinClampTo(divisionNumer, 0)][IMaxClampTo(l, shieldArray[AlienBullets[i]->data - 1]->Ydivisions - 1)] = false;
									shieldArray[AlienBullets[i]->data - 1]->Particles[IMinClampTo(--divisionNumer, 0)][IMaxClampTo(l + 2, shieldArray[AlienBullets[i]->data - 1]->Ydivisions - 1)] = false;
									shieldArray[AlienBullets[i]->data - 1]->Particles[IMinClampTo(divisionNumer, 0)][IMaxClampTo(l + 4, shieldArray[AlienBullets[i]->data - 1]->Ydivisions - 1)] = false;
									shieldArray[AlienBullets[i]->data - 1]->Particles[IMinClampTo(--divisionNumer, 0)][IMaxClampTo(l + 3, shieldArray[AlienBullets[i]->data - 1]->Ydivisions - 1)] = false;

									divisionNumer = OriginalX;
									l = OriginalY;
									shieldArray[AlienBullets[i]->data - 1]->Particles[IMaxClampTo(++divisionNumer, shieldArray[AlienBullets[i]->data - 1]->Xdivisions - 1)][IMaxClampTo(l, shieldArray[AlienBullets[i]->data - 1]->Ydivisions - 1)] = false;
									shieldArray[AlienBullets[i]->data - 1]->Particles[IMaxClampTo(divisionNumer, shieldArray[AlienBullets[i]->data - 1]->Xdivisions - 1)][IMaxClampTo(l + 1, shieldArray[AlienBullets[i]->data - 1]->Ydivisions - 1)] = false;
									shieldArray[AlienBullets[i]->data - 1]->Particles[IMaxClampTo(divisionNumer, shieldArray[AlienBullets[i]->data - 1]->Xdivisions - 1)][IMaxClampTo(l + 2, shieldArray[AlienBullets[i]->data - 1]->Ydivisions - 1)] = false;
									shieldArray[AlienBullets[i]->data - 1]->Particles[IMaxClampTo(divisionNumer, shieldArray[AlienBullets[i]->data - 1]->Xdivisions - 1)][IMaxClampTo(l + 3, shieldArray[AlienBullets[i]->data - 1]->Ydivisions - 1)] = false;
									shieldArray[AlienBullets[i]->data - 1]->Particles[IMaxClampTo(divisionNumer, shieldArray[AlienBullets[i]->data - 1]->Xdivisions - 1)][IMaxClampTo(l + 5, shieldArray[AlienBullets[i]->data - 1]->Ydivisions - 1)] = false;
									shieldArray[AlienBullets[i]->data - 1]->Particles[IMaxClampTo(divisionNumer, shieldArray[AlienBullets[i]->data - 1]->Xdivisions - 1)][IMaxClampTo(l + 7, shieldArray[AlienBullets[i]->data - 1]->Ydivisions - 1)] = false;
									shieldArray[AlienBullets[i]->data - 1]->Particles[IMaxClampTo(++divisionNumer, shieldArray[AlienBullets[i]->data - 1]->Xdivisions - 1)][IMaxClampTo(l + 2, shieldArray[AlienBullets[i]->data - 1]->Ydivisions - 1)] = false;
									shieldArray[AlienBullets[i]->data - 1]->Particles[IMaxClampTo(++divisionNumer, shieldArray[AlienBullets[i]->data - 1]->Xdivisions - 1)][IMaxClampTo(l + 4, shieldArray[AlienBullets[i]->data - 1]->Ydivisions - 1)] = false;


									divisionNumer = OriginalX;
									l = OriginalY;

									shieldArray[AlienBullets[i]->data - 1]->Particles[IMaxClampTo(++divisionNumer, 29)][IMinClampTo(--l, 0)] = false;
									shieldArray[AlienBullets[i]->data - 1]->Particles[IMaxClampTo(divisionNumer, 29)][IMaxClampTo(l++, shieldArray[AlienBullets[i]->data - 1]->Ydivisions - 1)] = false;
									shieldArray[AlienBullets[i]->data - 1]->Particles[IMaxClampTo(divisionNumer, 29)][IMaxClampTo(l++, shieldArray[AlienBullets[i]->data - 1]->Ydivisions - 1)] = false;
									shieldArray[AlienBullets[i]->data - 1]->Particles[IMaxClampTo(divisionNumer, 29)][IMaxClampTo(l++, shieldArray[AlienBullets[i]->data - 1]->Ydivisions - 1)] = false;
									shieldArray[AlienBullets[i]->data - 1]->Particles[IMaxClampTo(divisionNumer, 29)][IMaxClampTo(l++, shieldArray[AlienBullets[i]->data - 1]->Ydivisions - 1)] = false;
									shieldArray[AlienBullets[i]->data - 1]->Particles[IMaxClampTo(divisionNumer, 29)][IMaxClampTo(l++, shieldArray[AlienBullets[i]->data - 1]->Ydivisions - 1)] = false;
									shieldArray[AlienBullets[i]->data - 1]->Particles[IMaxClampTo(divisionNumer, 29)][IMaxClampTo(l + 1, shieldArray[AlienBullets[i]->data - 1]->Ydivisions - 1)] = false;

									int aliveParticleCount = 0;
									for (int k = 0; k < shieldArray[AlienBullets[i]->data - 1]->Xdivisions; k++)
									{
										for (int l = 0; l < shieldArray[AlienBullets[i]->data - 1]->Ydivisions; l++)
										{
											if (shieldArray[AlienBullets[i]->data - 1]->Particles[k][l] == true)
											{
												aliveParticleCount++;
											}
										}
									}

									if (aliveParticleCount <= (shieldArray[AlienBullets[i]->data - 1]->originalSize) * 0.6)
									{
										shieldArray[AlienBullets[i]->data - 1]->destroyed = true;
										CreateNewAnimation(NewVec2F(shieldArray[AlienBullets[i]->data - 1]->pos.x - shieldArray[AlienBullets[i]->data - 1]->dimensions.x / 2 + ShieldExplosion->frameWidth / 2,
											shieldArray[AlienBullets[i]->data - 1]->pos.y - shieldArray[AlienBullets[i]->data - 1]->dimensions.y + ShieldExplosion->frameHeight / 3 ),
											NewVec2F(0, 0), 0, ShieldExplosion, shieldArray[AlienBullets[i]->data - 1]->dimensions.x * 1.4, shieldArray[AlienBullets[i]->data - 1]->dimensions.y * 2);
									
										al_play_sample(ShieldDestroyed, 0.3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
									}

									CreateNewAnimation(NewVec2F(AlienBullets[i]->Pos.x + AlienBullets[i]->spriteS->frameWidth /4 - BulletExplosion->frameWidth / 2,
										AlienBullets[i]->Pos.y + AlienBullets[i]->spriteS->frameHeight / 5 - BulletExplosion->frameHeight / 2), NewVec2F(0, 0), 0, BulletExplosion, 50, 50);

									al_play_sample(ShieldImpact, 0.3, 0, 0.8, ALLEGRO_PLAYMODE_ONCE, NULL);

									DestroyAnimatedEntitySharedSprite(AlienBullets[i]);
									AlienBullets[i] = NULL;
									break;

								}
							}
						}
					}
				}
			}

		}
	}
}
#endif
#ifdef RASPI
void ComputeAlienShot()
{
	static float timeBuffer;

	timeBuffer += DeltaTime;

	if (timeBuffer >= ((AlienShootInterval * 0.75 + AlienShootInterval * 0.25 * 1/(Level + 1)) * TIME_MULTIPLIER))
	{
		timeBuffer = 0;

		float minimumDistanceToPlayer = 9999;
		float TempDistanceToPlayer;
		int closestAlienColumnToPlayer = (int)(AlienGrid->XAliens / 2);
		int lastAlienRow = 0;

		for (int i = 0; i < AlienGrid->XAliens; i++)
		{
			for (int j = (AlienGrid->YAliens - 1); j >= 0; j--)
			{
				if (AlienGrid->matrix[i][j] != NULL)
				{
					TempDistanceToPlayer = AbsoluteValue(AlienGrid->matrix[i][j]->Pos.x + (AlienGrid->matrix[i][j]->dimensions.x / 2) - (Spaceship->Pos.x + (Spaceship->dimensions.x / 2)));

					if (TempDistanceToPlayer < minimumDistanceToPlayer)
					{
						minimumDistanceToPlayer = TempDistanceToPlayer;
						closestAlienColumnToPlayer = i;
						lastAlienRow = j;
					}
					j = -1;
				}
			}
		}

		for (int i = 1; i < 15; i++)
		{
			if (AlienBullets[i] == NULL)
			{
				if (AlienGrid->matrix[closestAlienColumnToPlayer][lastAlienRow] != NULL)
				{
					AlienBullets[i] = CreateNewEntity(NewVec2(AlienGrid->matrix[closestAlienColumnToPlayer][lastAlienRow]->Pos.x + AlienGrid->matrix[closestAlienColumnToPlayer][lastAlienRow]->dimensions.x / 2, AlienGrid->matrix[closestAlienColumnToPlayer][lastAlienRow]->Pos.y + AlienGrid->matrix[closestAlienColumnToPlayer][lastAlienRow]->dimensions.y)
						, NewVec2(0, 1), (AlienShotSpeed * 0.75 + (AlienShotSpeed * 0.25 * 1 / (Level + 1))) * TIME_MULTIPLIER, BulletShape, NewVec2(1, 1));
					break;
				}

			}
		}
	}

}

int ColideAlienBullets()
{
	for (int i = 0; i < MAX_ALIEN_BULLETS; i++)
	{
		if (AlienBullets[i] != NULL)
		{
			for (int j = 0; j < numberOfShields; j++)
			{
				if (shieldArray[j] != NULL)
				{
					if (ColideAndDestroy(shieldArray[j], AlienBullets[i]))
					{
						DestroyEntity(AlienBullets[i]);
						AlienBullets[i] = NULL;

						return true;

					}
				}
			}
		}
	}
	return false;
}
#endif

void UpdateBullets()
{
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (Bullets[i] != NULL)
		{
			UpdateEntity(Bullets[i], DeltaTime);
		}
	}

	for (int i = 0; i < MAX_ALIEN_BULLETS; i++)
	{
		if (AlienBullets[i] != NULL)
		{
			UpdateEntity(AlienBullets[i], DeltaTime);
		}
	}
}

void CullBullets()
{
	for (int i = 0; i < 10; i++)
	{
		if (Bullets[i] != NULL)
		{
			if (Bullets[i]->Pos.y < 0)
			{
#ifndef RASPI
				DestroyEntityLoadedTexture(Bullets[i]);
#endif
#ifdef RASPI
				DestroyEntity(Bullets[i]);
#endif
				Bullets[i] = NULL;
			}
		}
	}

	for (int i = 0; i < 15; i++)
	{
		if (AlienBullets[i] != NULL)
		{
#ifndef RASPI
			if (AlienBullets[i]->Pos.y > ScreenDimensions.y)
			{
				DestroyAnimatedEntitySharedSprite(AlienBullets[i]);
				AlienBullets[i] = NULL;
			}
#endif
#ifdef RASPI
			if (AlienBullets[i]->Pos.y > 16)
			{
				DestroyEntity(AlienBullets[i]);
				AlienBullets[i] = NULL;
			}
#endif
		}
	}
}

void DrawBullets()
{
	for (int i = 1; i < MAX_BULLETS; i++)
	{
		if (Bullets[i] != NULL)
		{
			DrawEntity(Bullets[i]);
		}
	}

	for (int i = 1; i < MAX_ALIEN_BULLETS; i++)
	{
		if (AlienBullets[i] != NULL)
		{
			DrawEntity(AlienBullets[i]);
		}
	}
}

int AlienBulletsHit()
{

	for (int i = 1; i < 15; i++)
	{
		if (AlienBullets[i] != NULL)
		{
#ifndef RASPI
			if (AreColiding(AlienBullets[i], Spaceship))
			{
				CreateNewAnimation(NewVec2F(AlienBullets[i]->Pos.x + AlienBullets[i]->spriteS->frameWidth / 4 - BulletExplosion->frameWidth / 2,
					AlienBullets[i]->Pos.y + AlienBullets[i]->spriteS->frameHeight / 5 - BulletExplosion->frameHeight / 2), NewVec2F(0, 0), 0, BulletExplosion, 50, 50);

				al_play_sample(ShieldImpact, 0.3, 0, 0.8, ALLEGRO_PLAYMODE_ONCE, NULL);

				DestroyAnimatedEntitySharedSprite(AlienBullets[i]);
				AlienBullets[i] = NULL;
				lives--;

				return true;
			}
#endif
#ifdef RASPI
			if (AreColiding(AlienBullets[i], Spaceship))
			{
				DestroyEntity(AlienBullets[i]);
				AlienBullets[i] = NULL;
				lives--;
				return true;
			}
#endif
		}

	}
	return false;
}

void ProcessHP()
{
	if (lives <= 0)
	{
#ifndef RASPI
		Spaceship->data = 66;
		Gun->data = 66;
		CreateNewAnimation(Spaceship->Pos, NewVec2F(0, 0), 0, ShieldExplosion, 200, 300);
#endif
		GAMESTATE = END;
	}
}

int CollidePlayerBulletsWithShields()
{
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (Bullets[i] == NULL)
		{
			break;
		}
		for (int j = 0; j < numberOfShields; j++)
		{
			if (shieldArray[j] == NULL)
			{
				break;
			}
			if (AreColiding(Bullets[i] , shieldArray[j].))
		}
	}
}

#ifdef RASPI
void MotherShip()
{
	static int isMotherShipSpawned = false;
	if (Level != 0 && Level % 3 == 0 && isMotherShipSpawned == false && AlienGrid->Pos.y > 1 && wasMotherShipDestroyed == false)
	{
		char MotherShipShape[] = {
								  1,1,1,
								  1,0,1 };
		isMotherShipSpawned = true;
		BigUFOent = CreateNewEntity((Vec2) { 0, 0 }, (Vec2) { 1, 0 }, 0.2, MotherShipShape, (Vec2) { 3, 2 });
		PlayAudio(MOTHERSHIP_SFX_t);

	}
	if (isMotherShipSpawned == true)
	{
		UpdateEntity(BigUFOent, DeltaTime);
		if (BigUFOent->Pos.x > 16)
		{
			DestroyEntity(BigUFOent);
			BigUFOent = NULL;
			isMotherShipSpawned = false;
			wasMotherShipDestroyed = true;
		}
	}

	if (Level != 0 && Level % 3 != 0)
	{
		wasMotherShipDestroyed = false;
	}

	if (BigUFOent != NULL)
	{
		for (int i = 0; i < MAX_BULLETS; i++)
		{
			if (AreColiding(Bullets[i], BigUFOent))
			{
				DestroyEntity(BigUFOent);
				BigUFOent = NULL;
				isMotherShipSpawned = false;
				wasMotherShipDestroyed = true;

				BigUFOsDestroyed++;

			}
		}
	}
}
#endif