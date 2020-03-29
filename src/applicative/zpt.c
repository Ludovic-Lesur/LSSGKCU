/*
 * zpt.c
 *
 *  Created on: 26 mar. 2020
 *      Author: Ludo
 */

#include "zpt.h"

#include "mixer.h"
#include "sound.h"
#include "stdio.h"

/*** ZPT local global variables ***/

static SOUND_Context zpt_back_up_sound;
static SOUND_Context zpt_back_down_sound;
static SOUND_Context zpt_front_up_sound;
static SOUND_Context zpt_front_down_sound;

/*** ZPT functions ***/

/* INIT ZPT SOUND MODULE.
 * @param:	None.
 * @return:	None.
 */
void ZPT_Init(void) {
	// Init sounds.
	SOUND_Init(&zpt_back_up_sound, "zpt_up.wav", ZPT_BACK_AUDIO_GAIN);
	SOUND_SetVolume(&zpt_back_up_sound, 1.0); // No fade effect required.
	SOUND_Init(&zpt_back_down_sound, "zpt_down.wav", ZPT_BACK_AUDIO_GAIN);
	SOUND_SetVolume(&zpt_back_down_sound, 1.0); // No fade effect required.
	SOUND_Init(&zpt_front_up_sound, "zpt_up.wav", ZPT_FRONT_AUDIO_GAIN);
	SOUND_SetVolume(&zpt_front_up_sound, 1.0); // No fade effect required.
	SOUND_Init(&zpt_front_down_sound, "zpt_down.wav", ZPT_FRONT_AUDIO_GAIN);
	SOUND_SetVolume(&zpt_front_down_sound, 1.0); // No fade effect required.
}

/* RAISE BACK PANTOGRAPH.
 * @param:	None.
 * @eturn:	None.
 */
void ZPT_BackUp(void) {
	// Play sound.
	SOUND_Play(&zpt_back_up_sound);
	printf("ZPT *** Back up.\n");
	fflush(stdout);
}

/* LOWER BACK PANTOGRAPH.
 * @param:	None.
 * @eturn:	None.
 */
void ZPT_BackDown(void) {
	// Play sound.
	SOUND_Play(&zpt_back_down_sound);
	SOUND_Stop(&zpt_back_up_sound);
	printf("ZPT *** Back down.\n");
	fflush(stdout);
}

/* RAISE FRONT PANTOGRAPH.
 * @param:	None.
 * @eturn:	None.
 */
void ZPT_FrontUp(void) {
	// Play sound.
	SOUND_Play(&zpt_front_up_sound);
	printf("ZPT *** Front up.\n");
	fflush(stdout);
}

/* LOWER FRONT PANTOGRAPH.
 * @param:	None.
 * @eturn:	None.
 */
void ZPT_FrontDown(void) {
	// Play sound.
	SOUND_Play(&zpt_front_down_sound);
	SOUND_Stop(&zpt_front_up_sound);
	printf("ZPT *** Front down.\n");
	fflush(stdout);
}
