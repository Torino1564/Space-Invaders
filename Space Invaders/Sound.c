#include "Sound.h"

void InitAudio()
{
#ifndef RASPI_TEST
	al_install_audio();
	al_init_acodec_addon();

	al_reserve_samples(SAMPLE_COUNT);

	invaderDeath = al_load_sample(INVADER_DEATH);
	Shoot = al_load_sample(SHOOT_PATH);
	MenuMusic = al_load_sample(MENU_MUSIC);
	MotherShipSFX = al_load_sample(MOTHERSHIP_SFX);
	PlayerHit = al_load_sample(PLAYER_HIT);
	ShieldHit = al_load_sample(SHIELD_HIT);
#endif
#ifdef RASPI_TEST
	initAudio();
#endif
	return;	
}

void PlayAudio(int Sound)
{
#ifndef RASPI_TEST
	switch (Sound)
	{
	case INVADER_DEATH_t:
		al_play_sample(invaderDeath , 1 ,0 ,1,ALLEGRO_PLAYMODE_ONCE, NULL);
		break;
	case SHOOT_t:
		al_play_sample(Shoot, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
		break;
	case MENU_MUSIC_t:
		al_play_sample(MenuMusic, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
		break;
	case MOTHERSHIP_SFX_t:
		al_play_sample(MotherShipSFX, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
		break;
	case PLAYER_HIT_t:
		al_play_sample(PlayerHit, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
		break;
	case SHIELD_HIT_t:
		al_play_sample(ShieldHit, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
		break;

	}
#endif
#ifdef RASPI_TEST
	
	switch (Sound)
	{
	case INVADER_DEATH_t:
		playSound(INVADER_DEATH , SDL_MIX_MAXVOLUME / 2);
		break;
	case SHOOT_t:
		playSound(SHOOT_PATH , SDL_MIX_MAXVOLUME / 2);
		break;
	case MENU_MUSIC_t:
		playSound(MENU_MUSIC , SDL_MIX_MAXVOLUME / 2);
		break;
	case MOTHERSHIP_SFX_t:
		playSound(MOTHERSHIP_SFX , SDL_MIX_MAXVOLUME / 2);
		break;
	case PLAYER_HIT_t:
		playSound(PLAYER_HIT , SDL_MIX_MAXVOLUME / 2);
		break;
	case SHIELD_HIT_t:
		playSound(SHIELD_HIT , SDL_MIX_MAXVOLUME / 2);
		break;
	}
#endif
}

void stopAudio()
{
#ifndef RASPI_TEST
	al_stop_samples();
#endif
#ifdef RASPI_TEST
	endAudio();
#endif
}

void DestroyAudio()
{
#ifndef RASPI_TEST
	al_destroy_sample(invaderDeath);
	al_destroy_sample(Shoot);
	al_destroy_sample(MenuMusic);
	al_destroy_sample(PlayerHit);
	al_destroy_sample(MotherShipSFX);
#endif
}