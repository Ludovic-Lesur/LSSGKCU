/*
 * s.c
 *
 *  Created on: 10 may 2020
 *      Author: Ludo
 */

#include "s.h"

#include "mixer.h"
#include "openrails.h"
#include "sound.h"
#include "stdio.h"

/*** S local macros ***/

#define S_LOG

/*** S local structures ***/

typedef enum {
	S_STATE_HIGH_TONE,
	S_STATE_NEUTRAL,
	S_STATE_LOW_TONE
} S_State;

typedef struct {
	// Sounds.
	SOUND_Context s_low_tone_sound;
	SOUND_Context s_low_tone_end_sound;
	SOUND_Context s_high_tone_sound;
	SOUND_Context s_high_tone_end_sound;
	// State.
	S_State s_state;
} S_Context;

/*** S local global variables ***/

static S_Context s_ctx;

/*** S functions ***/

/* INIT S MODULE.
 * @param:	None.
 * @return:	None.
 */
void S_Init(void) {
	// Init sounds.
	SOUND_Init(&(s_ctx.s_low_tone_sound), "s_low_tone.wav", S_AUDIO_GAIN);
	SOUND_SetVolume(&(s_ctx.s_low_tone_sound), 1.0); // No fade effect required.
	SOUND_Init(&(s_ctx.s_low_tone_end_sound), "s_low_tone_end.wav", S_AUDIO_GAIN);
	SOUND_SetVolume(&(s_ctx.s_low_tone_end_sound), 1.0); // No fade effect required.
	SOUND_Init(&(s_ctx.s_high_tone_sound), "s_high_tone.wav", S_AUDIO_GAIN);
	SOUND_SetVolume(&(s_ctx.s_high_tone_sound), 1.0); // No fade effect required.
	SOUND_Init(&(s_ctx.s_high_tone_end_sound), "s_high_tone_end.wav", S_AUDIO_GAIN);
	SOUND_SetVolume(&(s_ctx.s_high_tone_end_sound), 1.0); // No fade effect required.
	// Init context.
	s_ctx.s_state = S_STATE_NEUTRAL;
}

/* PLAY HIGH TONE HORN.
 * @param:	None.
 * @return:	None.
 */
void S_HighTone(void) {
	// Play sound.
	SOUND_Play(&(s_ctx.s_high_tone_sound));
	// Update state.
	s_ctx.s_state = S_STATE_HIGH_TONE;
#ifdef S_LOG
	printf("S *** High tone.\n");
	fflush(stdout);
#endif
}

/* TURN HORN OFF.
 * @param:	None.
 * @eturn:	None.
 */
void S_Neutral(void) {
	// Check state.
	switch (s_ctx.s_state) {
	case S_STATE_LOW_TONE:
		// End low tone.
		SOUND_Play(&(s_ctx.s_low_tone_end_sound));
		SOUND_Stop(&(s_ctx.s_low_tone_sound));
		break;
	case S_STATE_HIGH_TONE:
		// End high tone.
		SOUND_Play(&(s_ctx.s_high_tone_end_sound));
		SOUND_Stop(&(s_ctx.s_high_tone_sound));
		break;
	default:
		break;
	}
	// Update state.
	s_ctx.s_state = S_STATE_NEUTRAL;
#ifdef S_LOG
	printf("S *** Neutral.\n");
	fflush(stdout);
#endif
}

/* PLAY LOW TONE HORN.
 * @param:	None.
 * @return:	None.
 */
void S_LowTone(void) {
	// Play sound.
	SOUND_Play(&(s_ctx.s_low_tone_sound));
	// Update state.
	s_ctx.s_state = S_STATE_LOW_TONE;
#ifdef S_LOG
	printf("S *** Low tone.\n");
	fflush(stdout);
#endif
}
