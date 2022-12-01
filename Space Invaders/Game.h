#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <stdio.h>
#include "Entity.h"
#include "Vec2.h"
#include <time.h>
#include "AlienMatrix.h"
/* ===================================

			FILE PATHS

====================================== */ 
//	Assets

#define ALIEN_TEXTURE1 "Resources/Assets/alien1.png"
#define BULLET_TEXTURE1 "Resources/Assets/bullet.png"
#define SHIP_TEXTURE	"Resources/Assets/Ship.png"

//	Backgrounds

#define MENU_BG	"Resources/Backgrounds/menulogo.png"
#define START_BG "Resources/Backgrounds/start.png"
#define LVL1_BG	"Resources/Backgrounds/bg1.png"

/* ===================================
	
		Functions

 ==================================== */
int SystemInit();
int Menu();
void Game();
int GameInit();
void GameLoop();
void GameLogic();
void GameRender();
void GameDestroy();
void Preframe();
void Postframe();

void CullBullets();
void UpdateBullets();

#define SHIP_SPEED 500

Vec2 ScreenDimensions;

static int running = 1;

typedef union ALLEGRO_EVENT ALLEGRO_EVENT;
typedef struct ALLEGRO_DISPLAY ALLEGRO_DISPLAY;
typedef struct ALLEGRO_EVENT_SOURCE ALLEGRO_EVENT_SOURCE;
typedef struct ALLEGRO_EVENT_QUEUE ALLEGRO_EVENT_QUEUE;
typedef struct ALLEGRO_TIMER ALLEGRO_TIMER;
typedef struct ALLEGRO_SAMPLE_ID ALLEGRO_SAMPLE_ID;
typedef struct ALLEGRO_SAMPLE ALLEGRO_SAMPLE;

ALLEGRO_DISPLAY* DISPLAY;

ALLEGRO_BITMAP* menu;

ALLEGRO_BITMAP* start;

ALLEGRO_BITMAP* background1;

ALLEGRO_EVENT_SOURCE* KeyboardEventSource;

ALLEGRO_EVENT_SOURCE* MouseEventSource;

ALLEGRO_EVENT_QUEUE* InputEventQueue;

ALLEGRO_EVENT TempEvent;

ALLEGRO_EVENT MenuEvent;

ALLEGRO_KEYBOARD_STATE KeyboardCurrentState;

Entity * Spaceship;

double DeltaTime;

clock_t t;

double PastFrameTime;

/* ==============================
				GUI
================================= */

Vec2 PlaySpaceArea;
Vec2 PlaySpacePos;
ALLEGRO_COLOR GUIColor;


/* ==============================
				Aliens
================================= */

Entity * Bullets[10];
ALLEGRO_BITMAP* BulletTexture;

AlienMatrix * AlienGrid;
ALLEGRO_BITMAP* AlienTexture;

Vec2 GridDimensions;
int AlienWidth;
int AlienHeight;

int XAliens;
int YAliens;

/* =============================
			Samples
=============================== */




