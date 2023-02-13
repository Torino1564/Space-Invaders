#pragma once
#ifdef RASPI_TEST
#include "Resources/libs/audio/SDL2/src/audio.h"
#include <SDL2/SDL.h>
#endif
#ifndef RASPI_TEST
#include "allegro5/allegro_audio.h"
#include "allegro5/allegro.h"
#include "allegro5/allegro5.h"
#include "allegro5/allegro_acodec.h"

#define SAMPLE_COUNT 30
#endif


void InitAudio();
void PlayAudio(int Song);
void stopAudio();
void DestroyAudio();

// Paths

#define INVADER_DEATH "Resources/Audio/Raspi/invaderkilled.wav"
#define SHOOT_PATH "Resources/Audio/Raspi/shoot.wav"
#define MENU_MUSIC "Resources/Audio/Raspi/spaceinvaders1.wav"
#define MOTHERSHIP_SFX "Resources/Audio/Raspi/ufo_lowpitch.wav"
#define PLAYER_HIT "Resources/Audio/Raspi/explosion.wav"
#define SHIELD_HIT "Resources/Audio/Raspi/fastinvader4.wav"

enum RaspiSFX{INVADER_DEATH_t, SHOOT_t,MENU_MUSIC_t,MOTHERSHIP_SFX_t,PLAYER_HIT_t , SHIELD_HIT_t};

#ifndef RASPI_TEST

ALLEGRO_SAMPLE* invaderDeath;
ALLEGRO_SAMPLE* Shoot;
ALLEGRO_SAMPLE* MenuMusic;
ALLEGRO_SAMPLE* MotherShipSFX;
ALLEGRO_SAMPLE* PlayerHit;
ALLEGRO_SAMPLE* ShieldHit;
#endif
