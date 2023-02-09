#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include "Entity.h"
#include "Vec2.h"
#include <time.h>
#include "AlienMatrix.h"
#include "Shield.h"
#include "Math.h"
#ifdef RASPI
#include "Input.h"
#endif

#ifndef RASPI
#include "Animation.h"
#include "SpriteSheet.h"


/* ===================================

			PLATFORM

====================================== */


/* ===================================

			FILE PATHS

====================================== */ 
//	Assets

#define ALIEN_TEXTURE1 "Resources/Assets/alien1.png"
#define BULLET_TEXTURE1 "Resources/Assets/bullet.png"
#define DEATH_TEXTURE "Resources/Assets/death.png"
#define EXPLOSION_SPRITE "Resources/Assets/MiniUFOExplosion.png"
#define SHIP_TEXTURE	"Resources/Assets/Ship.png"
#define GUN_TEXTURE "Resources/Assets/Gun.png"

#define MINIUFO1SP "Resources/Assets/MiniUFO1Sp.png"
#define SLUG "Resources/Assets/Spaceship_sprite.png"
#define SHIELD "Resources/Assets/ShieldTexture2.png"
#define ALIENBULLETS "Resources/Assets/AlienBullet.png"
#define BULLET_EXPLOSION_SS "Resources/Assets/bullet_Explosion_spritesheet.png"
#define SHIELD_EXPLOSION_SS "Resources/Assets/shieldExplosion.png"
#define FONT "Resources/Assets/PublicPixel-z84yD.ttf"
#define HEART "Resources/Assets/heart.png"
#define DHEART "Resources/Assets/deadheart.png"

#define STOP_BACKWARDS "Resources/Assets/Stopping_backwards.png"
#define STOP_FORWARDS "Resources/Assets/Stopping_forwards.png"

//	Backgrounds

#define MENU_BG	"Resources/Backgrounds/menulogo.png"
#define START_BG "Resources/Backgrounds/start.png"

#define LVL1_BG	"Resources/Backgrounds/bg1.png"
#define LVL2_BG	"Resources/Backgrounds/bg2.png"
#define LVL3_BG	"Resources/Backgrounds/bg3.png"
#define LVL4_BG	"Resources/Backgrounds/bg4.png"


#define PAUSE_BG "Resources/Backgrounds/pause.png"
#define LIGHTGRAYOVERLAY "Resources/Backgrounds/LightGrayOverlay.png"
#endif
#ifdef RASPI
#include "Graphics.h"
#endif
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
void SystemDestroy();
void Preframe();
void Postframe();
void Pause();
void EndScreen();
void UpdateBullets();
void CullBullets();
void ComputeAlienShot();
void DrawBullets();
int AlienBulletsHit();
void ProcessHP();
#ifndef RASPI
void AnimateBullets();
void CollideAlienBullets();
#endif
#ifdef RASPI
void ColideAlienBullets();

#endif

Entity* Spaceship;

#ifndef RASPI
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
ALLEGRO_BITMAP* background2;
ALLEGRO_BITMAP* background3;
ALLEGRO_BITMAP* background4;
ALLEGRO_BITMAP* backgroundpause;
ALLEGRO_BITMAP* LightGrayOverlay;

ALLEGRO_BITMAP* heart;
ALLEGRO_BITMAP* deadheart;

ALLEGRO_EVENT_SOURCE* KeyboardEventSource;

ALLEGRO_EVENT_SOURCE* MouseEventSource;

ALLEGRO_EVENT_QUEUE* InputEventQueue;

ALLEGRO_EVENT TempEvent;

ALLEGRO_EVENT MenuEvent;

ALLEGRO_KEYBOARD_STATE KeyboardCurrentState;

Entity* Gun;

Entity Marco;
#endif

double DeltaTime;

clock_t t;

double PastFrameTime;

/* ==============================
				GUI
================================= */
#ifndef RASPI
Vec2 PlaySpaceArea;
Vec2 PlaySpacePos;
ALLEGRO_COLOR GUIColor;
ALLEGRO_FONT* font;
ALLEGRO_FONT* BigFont;
#endif
#ifdef RASPI

#endif
/* ==============================
			Game Elements
================================= */

#define BASE_ALIEN_SHOT_SPEED 2.5
#define MAX_BULLETS 10
#define MAX_ALIEN_BULLETS 15

Entity * Bullets[MAX_BULLETS];
Entity* AlienBullets[MAX_ALIEN_BULLETS];
#ifndef RASPI
ALLEGRO_BITMAP* BulletTexture;
ALLEGRO_BITMAP* DeathTexture;
#endif
int numberOfShields;
#ifndef RASPI
shield* shieldArray[10];
shield* shield1;
Vec2F shieldSize;
float shieldPadding;
#endif
#ifdef RASPI
Entity* shieldArray[4];
#endif

#ifdef RASPI

char BulletShape[1];

#endif

AlienMatrix * AlienGrid;
#ifndef RASPI
ALLEGRO_BITMAP* AlienTexture;

SpriteSheet* MiniUFO;
SpriteSheet* Slug;
SpriteSheet* Weapon;
SpriteSheet* AlienBullet;
SpriteSheet* MiniUFO_Explosion;
SpriteSheet* BulletExplosion;
SpriteSheet* ShieldExplosion;

SpriteSheet* Stopping_b;
SpriteSheet* Stopping_f;

ALLEGRO_BITMAP* ShieldTexture;
#endif
Vec2 GridDimensions;
int AlienWidth;
int AlienHeight;

int XAliens;
int YAliens;

/* =============================
			Audio
=============================== */

#define SAMPLE_COUNT 30
#ifndef RASPI
#define BACKGROUNDMUSIC1 "Resources/Audio/spaceinvaders1.mpeg"
#define BACKGROUNDMUSIC2 NULL

#define ALIENDEATHSFX "Resources/Audio/AlienExplosion2.wav"
#define ALIEN1SFX	"Resources/Audio/fastinvader1.wav"
#define ALIEN2SFX	"Resources/Audio/fastinvader2.wav"
#define ALIEN3SFX	"Resources/Audio/fastinvader3.wav"
#define ALIEN4SFX	"Resources/Audio/fastinvader4.wav"

#define EXPLOSION1	"Resources/Audio/Explosion1.wav"

#define MUSIC_LEVEL1 "Resources/Audio/Level1.mp3" 
#define MUSIC_LEVEL2 "Resources/Audio/Level2.mp3" 
#define MUSIC_LEVEL3 "Resources/Audio/Level3.mp3" 
#define MUSIC_LEVEL4 "Resources/Audio/Level4.mp3" 

#define PLAYERSHOTSFX "Resources/Audio/shoot.mp3"
#define SHOTNOTREADY "Resources/Audio/ShotNotReady.wav"
#define PLAYERIMPACTSFX "Resources/Audio/ShipHit.wav"
#define PLAYERDEATHSFX NULL

#define SHIELD_IMPACT "Resources/Audio/shieldImpact.wav"
#define SHIELD_EXPLOSION "Resources/Audio/ShieldExplosion.wav"

ALLEGRO_MIXER* Mixer;

ALLEGRO_SAMPLE* BackgroundMusic1;
ALLEGRO_SAMPLE* BackgroundMusic2;

ALLEGRO_SAMPLE* AlienDeathSFX;
ALLEGRO_SAMPLE* Alien1SFX;
ALLEGRO_SAMPLE* Alien2SFX;
ALLEGRO_SAMPLE* Alien3SFX;
ALLEGRO_SAMPLE* Alien4SFX;

ALLEGRO_SAMPLE* Explosion1;

ALLEGRO_SAMPLE* PlayerShotSFX;
ALLEGRO_SAMPLE* ShotNotReadySFX;
ALLEGRO_SAMPLE* ShipImpactSFX;
ALLEGRO_SAMPLE* PlayerDeathSFX;

ALLEGRO_SAMPLE* ShieldImpact;
ALLEGRO_SAMPLE* ShieldDestroyed;

ALLEGRO_SAMPLE* menuMusic;
ALLEGRO_SAMPLE* level1Music;
ALLEGRO_SAMPLE* level2Music;
ALLEGRO_SAMPLE* level3Music;
ALLEGRO_SAMPLE* level4Music;


ALLEGRO_SAMPLE* Bullet_sound;
ALLEGRO_SAMPLE* alien_death_sound;

ALLEGRO_SAMPLE_INSTANCE* instance1;
#endif

/* =============================
			ESTADOS
=============================== */
int ESTADO;

enum { MENU, GAME , STOP };

int GAMESTATE;

enum GAMESTATE {PLAYING , PAUSE, END , EXIT};


/* =============================
			global variables
=============================== */

int pause;
int Level;
int Once;
int Moving;
int shot;
int shotOnCooldown;
int aliendeath;
int lives;
int score;
int aliensDestroyed;