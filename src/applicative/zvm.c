/*
 * zvm.c
 *
 *  Created on: 25 mars 2020
 *      Author: Ludovic
 */

#include "zvm.h"

#include "mixer.h"
#include "sound.h"
#include "stdio.h"

/*** ZVM local macros ***/

#define ZVM_FADE_DURATION_MS	1000
#define ZVM_FADE_MARGIN_MS		1000 // Added to fade duration.

/*** ZVM local stuctures ***/

typedef enum {
	ZVM_STATE_OFF,
	ZVM_STATE_TURNON,
	ZVM_STATE_TURNON_TO_ON1,
	ZVM_STATE_ON1,
	ZVM_STATE_ON1_TO_ON2,
	ZVM_STATE_ON2,
	ZVM_STATE_ON2_TO_ON1,
	ZVM_STATE_TURNOFF
} ZVM_State;

typedef struct {
	SOUND_Context zvm_sound_turnon;
	SOUND_Context zvm_sound_on1;
	SOUND_Context zvm_sound_on2;
	SOUND_Context zvm_sound_turnoff;
	unsigned char zvm_on;
	ZVM_State zvm_state;
} ZVM_Context;

/*** ZVM local global variables ***/

static ZVM_Context zvm_ctx;

/*** ZVM functions ***/

/* INIT ZVM SOUND MODULE.
 * @param:	None.
 * @return:	None.
 */
void ZVM_Init(void) {
	// Init sounds.
	SOUND_Init(&(zvm_ctx.zvm_sound_turnon), "C:/Users/Ludovic/Documents/Eclipse/LSSGKCU/wav/ventilateurs_allumage.wav", ZVM_MAXIMUM_VOLUME);
	SOUND_Init(&(zvm_ctx.zvm_sound_on1), "C:/Users/Ludovic/Documents/Eclipse/LSSGKCU/wav/ventilateurs_marche.wav", ZVM_MAXIMUM_VOLUME);
	SOUND_Init(&(zvm_ctx.zvm_sound_on2), "C:/Users/Ludovic/Documents/Eclipse/LSSGKCU/wav/ventilateurs_marche.wav", ZVM_MAXIMUM_VOLUME);
	SOUND_Init(&(zvm_ctx.zvm_sound_turnoff), "C:/Users/Ludovic/Documents/Eclipse/LSSGKCU/wav/ventilateurs_extinction.wav", ZVM_MAXIMUM_VOLUME);
	// Init state machine.
	zvm_ctx.zvm_on = 0;
	zvm_ctx.zvm_state = ZVM_STATE_OFF;
}

/* TURN MOTORS FANS ON.
 * @param:	None.
 * @return:	None.
 */
void ZVM_TurnOn(void) {
	zvm_ctx.zvm_on = 1;
	printf("ZVM *** Turn on.\n");
	fflush(stdout);
}

/* TURN MOTORS FANS OFF.
 * @param:	None.
 * @return:	None.
 */
void ZVM_TurnOff(void) {
	zvm_ctx.zvm_on = 0;
	printf("ZVM *** Turn off.\n");
	fflush(stdout);
}

/* MAIN TASK OF ZVM SOUND MODULE.
 * @param:	None.
 * @return:	None.
 */
void ZVM_Task(void) {
	// Local variables.
	unsigned char zvm_turnon_fade_end = 0;
	unsigned char zvm_on1_fade_end = 0;
	unsigned char zvm_on2_fade_end = 0;
	unsigned char zvm_turnoff_fade_end = 0;
	// Perform internal state machine.
	switch (zvm_ctx.zvm_state) {
	case ZVM_STATE_OFF:
		if (zvm_ctx.zvm_on != 0) {
			// Start playing turn-on sound.
			SOUND_SetVolume(&(zvm_ctx.zvm_sound_turnon), 1.0); // No fade-in effect required.
			SOUND_Play(&(zvm_ctx.zvm_sound_turnon));
			zvm_ctx.zvm_state = ZVM_STATE_TURNON;
		}
		break;
	case ZVM_STATE_TURNON:
		if (zvm_ctx.zvm_on == 0) {
			// Save all other sounds volume.
			SOUND_SaveFadeParameters(&(zvm_ctx.zvm_sound_turnon));
			SOUND_SaveFadeParameters(&(zvm_ctx.zvm_sound_on1));
			SOUND_SaveFadeParameters(&(zvm_ctx.zvm_sound_on2));
			// Start playing turn-off sound.
			SOUND_SetVolume(&(zvm_ctx.zvm_sound_turnoff), 0.0);
			SOUND_Play(&(zvm_ctx.zvm_sound_turnoff));
			SOUND_SaveFadeParameters(&(zvm_ctx.zvm_sound_turnoff));
			zvm_ctx.zvm_state = ZVM_STATE_TURNOFF;
		}
		else {
			if ((SOUND_GetPositionMs(&(zvm_ctx.zvm_sound_turnon))) > ((SOUND_GetLengthMs(&(zvm_ctx.zvm_sound_turnon))) - ZVM_FADE_DURATION_MS - ZVM_FADE_MARGIN_MS)) {
				// Save turn-on volume.
				SOUND_SaveFadeParameters(&(zvm_ctx.zvm_sound_turnon));
				// Start playing On1 sound.
				SOUND_SetVolume(&(zvm_ctx.zvm_sound_on1), 0.0);
				SOUND_Play(&(zvm_ctx.zvm_sound_on1));
				SOUND_SaveFadeParameters(&(zvm_ctx.zvm_sound_on1));
				// Stop playing turn-off sound (in case it was running).
				SOUND_Stop(&(zvm_ctx.zvm_sound_turnoff));
				zvm_ctx.zvm_state = ZVM_STATE_TURNON_TO_ON1;
			}
		}
		break;
	case ZVM_STATE_TURNON_TO_ON1:
		if (zvm_ctx.zvm_on == 0) {
			// Save all other sounds volume.
			SOUND_SaveFadeParameters(&(zvm_ctx.zvm_sound_turnon));
			SOUND_SaveFadeParameters(&(zvm_ctx.zvm_sound_on1));
			SOUND_SaveFadeParameters(&(zvm_ctx.zvm_sound_on2));
			// Start playing turn-off sound.
			SOUND_SetVolume(&(zvm_ctx.zvm_sound_turnoff), 0.0);
			SOUND_Play(&(zvm_ctx.zvm_sound_turnoff));
			SOUND_SaveFadeParameters(&(zvm_ctx.zvm_sound_turnoff));
			zvm_ctx.zvm_state = ZVM_STATE_TURNOFF;
		}
		else {
			// Perform turn-on fade-out and On1 fade-in.
			zvm_on1_fade_end = SOUND_FadeIn(&(zvm_ctx.zvm_sound_on1), ZVM_FADE_DURATION_MS);
			zvm_turnon_fade_end = SOUND_FadeOut(&(zvm_ctx.zvm_sound_turnon), ZVM_FADE_DURATION_MS);
			// Change state when effect is complete.
			if ((zvm_on1_fade_end > 0) && (zvm_turnon_fade_end > 0)) {
				// Stop turn-on sound.
				SOUND_Stop(&(zvm_ctx.zvm_sound_turnon));
				zvm_ctx.zvm_state = ZVM_STATE_ON1;
			}
		}
		break;
	case ZVM_STATE_ON1:
		if (zvm_ctx.zvm_on == 0) {
			// Save all other sounds volume.
			SOUND_SaveFadeParameters(&(zvm_ctx.zvm_sound_turnon));
			SOUND_SaveFadeParameters(&(zvm_ctx.zvm_sound_on1));
			SOUND_SaveFadeParameters(&(zvm_ctx.zvm_sound_on2));
			// Start playing turn-off sound.
			SOUND_SetVolume(&(zvm_ctx.zvm_sound_turnoff), 0.0);
			SOUND_Play(&(zvm_ctx.zvm_sound_turnoff));
			SOUND_SaveFadeParameters(&(zvm_ctx.zvm_sound_turnoff));
			zvm_ctx.zvm_state = ZVM_STATE_TURNOFF;
		}
		else {
			if ((SOUND_GetPositionMs(&(zvm_ctx.zvm_sound_on1))) > ((SOUND_GetLengthMs(&(zvm_ctx.zvm_sound_on1))) - ZVM_FADE_DURATION_MS - ZVM_FADE_MARGIN_MS)) {
				// Save On1 volume.
				SOUND_SaveFadeParameters(&(zvm_ctx.zvm_sound_on1));
				// Start playing On2 sound.
				SOUND_SetVolume(&(zvm_ctx.zvm_sound_on2), 0.0);
				SOUND_Play(&(zvm_ctx.zvm_sound_on2));
				SOUND_SaveFadeParameters(&(zvm_ctx.zvm_sound_on2));
				zvm_ctx.zvm_state = ZVM_STATE_ON1_TO_ON2;
			}
		}
		break;
	case ZVM_STATE_ON1_TO_ON2:
		if (zvm_ctx.zvm_on == 0) {
			// Save all other sounds volume.
			SOUND_SaveFadeParameters(&(zvm_ctx.zvm_sound_turnon));
			SOUND_SaveFadeParameters(&(zvm_ctx.zvm_sound_on1));
			SOUND_SaveFadeParameters(&(zvm_ctx.zvm_sound_on2));
			// Start playing turn-off sound.
			SOUND_SetVolume(&(zvm_ctx.zvm_sound_turnoff), 0.0);
			SOUND_Play(&(zvm_ctx.zvm_sound_turnoff));
			SOUND_SaveFadeParameters(&(zvm_ctx.zvm_sound_turnoff));
			zvm_ctx.zvm_state = ZVM_STATE_TURNOFF;
		}
		else {
			// Perform On1 fade-out and On2 fade-in.
			zvm_on2_fade_end = SOUND_FadeIn(&(zvm_ctx.zvm_sound_on2), ZVM_FADE_DURATION_MS);
			zvm_on1_fade_end = SOUND_FadeOut(&(zvm_ctx.zvm_sound_on1), ZVM_FADE_DURATION_MS);
			// Change state when effect is complete.
			if ((zvm_on2_fade_end > 0) && (zvm_on1_fade_end > 0)) {
				// Stop On1 sound.
				SOUND_Stop(&(zvm_ctx.zvm_sound_on1));
				zvm_ctx.zvm_state = ZVM_STATE_ON2;
			}
		}
		break;
	case ZVM_STATE_ON2:
		if (zvm_ctx.zvm_on == 0) {
			// Save all other sounds volume.
			SOUND_SaveFadeParameters(&(zvm_ctx.zvm_sound_turnon));
			SOUND_SaveFadeParameters(&(zvm_ctx.zvm_sound_on1));
			SOUND_SaveFadeParameters(&(zvm_ctx.zvm_sound_on2));
			// Start playing turn-off sound.
			SOUND_SetVolume(&(zvm_ctx.zvm_sound_turnoff), 0.0);
			SOUND_Play(&(zvm_ctx.zvm_sound_turnoff));
			SOUND_SaveFadeParameters(&(zvm_ctx.zvm_sound_turnoff));
			zvm_ctx.zvm_state = ZVM_STATE_TURNOFF;
		}
		else {
			if ((SOUND_GetPositionMs(&(zvm_ctx.zvm_sound_on2))) > ((SOUND_GetLengthMs(&(zvm_ctx.zvm_sound_on2))) - ZVM_FADE_DURATION_MS - ZVM_FADE_MARGIN_MS)) {
				// Save On2 volume.
				SOUND_SaveFadeParameters(&(zvm_ctx.zvm_sound_on2));
				// Start playing On1 sound.
				SOUND_SetVolume(&(zvm_ctx.zvm_sound_on1), 0.0);
				SOUND_Play(&(zvm_ctx.zvm_sound_on1));
				SOUND_SaveFadeParameters(&(zvm_ctx.zvm_sound_on1));
				zvm_ctx.zvm_state = ZVM_STATE_ON2_TO_ON1;
			}
		}
		break;
	case ZVM_STATE_ON2_TO_ON1:
		if (zvm_ctx.zvm_on == 0) {
			// Save all other sounds volume.
			SOUND_SaveFadeParameters(&(zvm_ctx.zvm_sound_turnon));
			SOUND_SaveFadeParameters(&(zvm_ctx.zvm_sound_on1));
			SOUND_SaveFadeParameters(&(zvm_ctx.zvm_sound_on2));
			// Start playing turn-off sound.
			SOUND_SetVolume(&(zvm_ctx.zvm_sound_turnoff), 0.0);
			SOUND_Play(&(zvm_ctx.zvm_sound_turnoff));
			SOUND_SaveFadeParameters(&(zvm_ctx.zvm_sound_turnoff));
			zvm_ctx.zvm_state = ZVM_STATE_TURNOFF;
		}
		else {
			// Perform On2 fade-out and On1 fade-in.
			zvm_on1_fade_end = SOUND_FadeIn(&(zvm_ctx.zvm_sound_on1), ZVM_FADE_DURATION_MS);
			zvm_on2_fade_end = SOUND_FadeOut(&(zvm_ctx.zvm_sound_on2), ZVM_FADE_DURATION_MS);
			// Change state when effect is complete.
			if ((zvm_on1_fade_end > 0) && (zvm_on2_fade_end > 0)) {
				// Stop On2 sound.
				SOUND_Stop(&(zvm_ctx.zvm_sound_on2));
				zvm_ctx.zvm_state = ZVM_STATE_ON1;
			}
		}
		break;
	case ZVM_STATE_TURNOFF:
		// Perform On1, On2 and turn-on fade-out and turn-off fade-in.
		zvm_turnoff_fade_end = SOUND_FadeIn(&(zvm_ctx.zvm_sound_turnoff), ZVM_FADE_DURATION_MS);
		zvm_turnon_fade_end = SOUND_FadeOut(&(zvm_ctx.zvm_sound_turnon), ZVM_FADE_DURATION_MS);
		zvm_on1_fade_end = SOUND_FadeOut(&(zvm_ctx.zvm_sound_on1), ZVM_FADE_DURATION_MS);
		zvm_on2_fade_end = SOUND_FadeOut(&(zvm_ctx.zvm_sound_on2), ZVM_FADE_DURATION_MS);
		// Change state when effect is complete.
		if ((zvm_turnoff_fade_end > 0) && (zvm_turnon_fade_end > 0) && (zvm_on1_fade_end > 0) && (zvm_on2_fade_end > 0)) {
			// Stop TurnOn, On1 and On2 sounds.
			SOUND_Stop(&(zvm_ctx.zvm_sound_turnon));
			SOUND_Stop(&(zvm_ctx.zvm_sound_on1));
			SOUND_Stop(&(zvm_ctx.zvm_sound_on2));
			zvm_ctx.zvm_state = ZVM_STATE_OFF;
		}
		break;
	// Unknown state.
	default:
		break;
	}
}
