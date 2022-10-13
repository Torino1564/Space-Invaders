#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include "Entity.h"
#include "Vec2.h"
#include <time.h>
#include "AlienMatrix.h"

int Menu();
void Game();
/* ===================================
	
		Functions

 ==================================== */
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

ALLEGRO_DISPLAY* DISPLAY;

ALLEGRO_BITMAP* menu0; 
ALLEGRO_BITMAP* menu1;

ALLEGRO_BITMAP* background1;

ALLEGRO_EVENT_SOURCE* KeyboardEventSource;

ALLEGRO_EVENT_SOURCE* MouseEventSource;

ALLEGRO_EVENT_QUEUE* InputEventQueue;

ALLEGRO_EVENT TempEvent;

ALLEGRO_KEYBOARD_STATE KeyboardCurrentState;

Entity * Spaceship;

double DeltaTime;

clock_t t;

double FramesPerSecond;

/* ==============================
				Aliens
================================= */

Entity* Alien;

Entity * Bullets[10];
ALLEGRO_BITMAP* BulletTexture;

AlienMatrix * AlienGrid;
ALLEGRO_BITMAP* AlienTexture;

int XAliens;
int YAliens;