/*
 * zba.c
 *
 *  Created on: 26 mar. 2020
 *      Author: Ludo
 */

#include "zba.h"

#include "mixer.h"
#include "sound.h"
#include "stdio.h"

/*** ZBA local global variables ***/

static SOUND_Context zba_sound;

/*** ZBA functions ***/

/* INIT ZBA SOUND MODULE.
 * @param:	None.
 * @return:	None.
 */
void ZBA_Init(void) {
	// Init sound.
	SOUND_Init(&zba_sound, "zba.wav", ZBA_AUDIO_GAIN);
	SOUND_SetVolume(&zba_sound, 1.0); // No fade effect required.
}

/* TURN ZBA ON.
 * @param:	None.
 * @return:	None.
 */
void ZBA_TurnOn(void) {
	// Play sound.
	SOUND_Play(&zba_sound);
	printf("ZBA *** Turn on.\n");
	fflush(stdout);
}

/* TURN ZBA OFF.
 * @param:	None.
 * @return:	None.
 */
void ZBA_TurnOff(void) {
	// Play sound.
	SOUND_Play(&zba_sound);
	printf("ZBA *** Turn off.\n");
	fflush(stdout);
}
