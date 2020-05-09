/*
 * comp.c
 *
 *  Created on: 8 may 2020
 *      Author: Ludo
 */

#include "comp.h"

#include "mixer.h"
#include "sound.h"
#include "stdio.h"

/*** COMP local macros ***/

#define COMP_FADE_DURATION_MS	1000
#define COMP_FADE_MARGIN_MS		1000 // Added to fade duration.
#define COMP_AUTO_OFF_MARGIN_MS	2000 // Automatically return to off state when sound position reaches its duration minus this margin.

/*** COMP local structures ***/

typedef enum {
	COMP_STATE_OFF,
	COMP_STATE_AUTO_REG_MIN_MAX,
	COMP_STATE_AUTO_TURNOFF,
	COMP_STATE_DIRECT_TURNON,
	COMP_STATE_DIRECT_TURNON_TO_ON1,
	COMP_STATE_DIRECT_ON1,
	COMP_STATE_DIRECT_ON1_TO_ON2,
	COMP_STATE_DIRECT_ON2,
	COMP_STATE_DIRECT_ON2_TO_ON1,
	COMP_STATE_DIRECT_TURNOFF
} COMP_State;

typedef enum {
	COMP_SOUND_REQUEST_REG_MIN,
	COMP_SOUND_REQUEST_REG_MAX,
	COMP_SOUND_REQUEST_DIRECT,
	COMP_SOUND_REQUEST_OFF
} COMP_SoundRequest;

typedef struct {
	// Sounds.
	SOUND_Context comp_sound_auto_reg_min;
	SOUND_Context comp_sound_auto_reg_max;
	SOUND_Context comp_sound_auto_turnoff;
	SOUND_Context comp_sound_direct_turnon;
	SOUND_Context comp_sound_direct_on1;
	SOUND_Context comp_sound_direct_on2;
	SOUND_Context comp_sound_direct_turnoff;
	// State machine.
	COMP_State comp_state;
	COMP_SoundRequest comp_sound_request;
} COMP_Context;

/*** COMP local global variables ***/

static COMP_Context comp_ctx;

/*** COMP functions ***/

/* INIT COMPRESSOR SOUND MODULE.
 * @param:	None.
 * @return:	None.
 */
void COMP_Init(void) {
	// Init sounds.
	SOUND_Init(&(comp_ctx.comp_sound_auto_reg_min), "comp_auto_reg_min.wav", COMP_AUDIO_GAIN);
	SOUND_Init(&(comp_ctx.comp_sound_auto_reg_max), "comp_auto_reg_max.wav", COMP_AUDIO_GAIN);
	SOUND_Init(&(comp_ctx.comp_sound_auto_turnoff), "comp_auto_turnoff.wav", COMP_AUDIO_GAIN);
	SOUND_Init(&(comp_ctx.comp_sound_direct_turnon), "comp_direct_turnon.wav", COMP_AUDIO_GAIN);
	SOUND_Init(&(comp_ctx.comp_sound_direct_on1), "comp_direct_on.wav", COMP_AUDIO_GAIN);
	SOUND_Init(&(comp_ctx.comp_sound_direct_on2), "comp_direct_on.wav", COMP_AUDIO_GAIN);
	SOUND_Init(&(comp_ctx.comp_sound_direct_turnoff), "comp_direct_turnoff.wav", COMP_AUDIO_GAIN);
	// Init state machine.
	comp_ctx.comp_state = COMP_STATE_OFF;
	comp_ctx.comp_sound_request = COMP_SOUND_REQUEST_OFF;
}

/* PLAY MINIMUM REGULATION SOUND.
 * @param:	None.
 * @return:	None.
 */
void COMP_PlayAutoRegulationMin(void) {
	comp_ctx.comp_sound_request = COMP_SOUND_REQUEST_REG_MIN;
	printf("COMP *** Regulation min request.\n");
	fflush(stdout);
}

/* PLAY MAXIMUM REGULATION SOUND.
 * @param:	None.
 * @return:	None.
 */
void COMP_PlayAutoRegulationMax(void) {
	comp_ctx.comp_sound_request = COMP_SOUND_REQUEST_REG_MAX;
	printf("COMP *** Regulation max request.\n");
	fflush(stdout);
}

/* PLAY DIRECT COMPRESSOR SOUND.
 * @param:	None.
 * @return:	None.
 */
void COMP_PlayDirect(void) {
	comp_ctx.comp_sound_request = COMP_SOUND_REQUEST_DIRECT;
	printf("COMP *** Direct request.\n");
	fflush(stdout);
}

/* PLAY TURN-OFF SOUND.
 * @param:	None.
 * @return:	None.
 */
void COMP_TurnOff(void) {
	comp_ctx.comp_sound_request = COMP_SOUND_REQUEST_OFF;
	printf("COMP *** Regulation turn-off request.\n");
	fflush(stdout);
}

/* MAIN TASK OF COMPRESSOR SOUND MODULE.
 * @param:	None.
 * @return:	None.
 */
void COMP_Task(void) {
	// Local variables.
	unsigned char comp_auto_reg_min_fade_end = 0;
	unsigned char comp_auto_reg_max_fade_end = 0;
	unsigned char comp_auto_turnoff_fade_end = 0;
	unsigned char comp_direct_turnon_fade_end = 0;
	unsigned char comp_direct_on1_fade_end = 0;
	unsigned char comp_direct_on2_fade_end = 0;
	unsigned char comp_direct_turnoff_fade_end = 0;
	// Perform internal state machine.
	switch (comp_ctx.comp_state) {
	case COMP_STATE_OFF:
		if (comp_ctx.comp_sound_request == COMP_SOUND_REQUEST_DIRECT) {
			// Start playing turn-on sound.
			SOUND_SetVolume(&(comp_ctx.comp_sound_direct_turnon), 1.0); // No fade-in effect required.
			SOUND_Play(&(comp_ctx.comp_sound_direct_turnon));
			SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_direct_turnon));
			comp_ctx.comp_state = COMP_STATE_DIRECT_TURNON;
		}
		else {
			if (comp_ctx.comp_sound_request == COMP_SOUND_REQUEST_REG_MIN) {
				// Start playing minimum regulation sound.
				SOUND_SetVolume(&(comp_ctx.comp_sound_auto_reg_min), 1.0); // No fade-in effect required.
				SOUND_Play(&(comp_ctx.comp_sound_auto_reg_min));
				SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_auto_reg_min));
				comp_ctx.comp_state = COMP_STATE_AUTO_REG_MIN_MAX;
			}
			else {
				if (comp_ctx.comp_sound_request == COMP_SOUND_REQUEST_REG_MAX) {
					// Start playing maximum regulation sound.
					SOUND_SetVolume(&(comp_ctx.comp_sound_auto_reg_max), 1.0); // No fade-in effect required.
					SOUND_Play(&(comp_ctx.comp_sound_auto_reg_max));
					SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_auto_reg_max));
					comp_ctx.comp_state = COMP_STATE_AUTO_REG_MIN_MAX;
				}
			}
		}
		break;
	case COMP_STATE_AUTO_REG_MIN_MAX:
		// Auto to direct mode.
		if (comp_ctx.comp_sound_request == COMP_SOUND_REQUEST_DIRECT) {
			// Save all auto sounds.
			SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_auto_reg_min));
			SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_auto_reg_max));
			SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_auto_turnoff));
			// Start playing turn-on sound.
			SOUND_SetVolume(&(comp_ctx.comp_sound_direct_turnon), 1.0); // No fade-in effect required.
			SOUND_Play(&(comp_ctx.comp_sound_direct_turnon));
			SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_direct_turnon));
			comp_ctx.comp_state = COMP_STATE_DIRECT_TURNON;
		}
		else {
			if (comp_ctx.comp_sound_request == COMP_SOUND_REQUEST_OFF) {
				// Save all regulation sounds.
				SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_auto_reg_min));
				SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_auto_reg_max));
				// Start playing turn-off sound.
				SOUND_SetVolume(&(comp_ctx.comp_sound_auto_turnoff), 0.0);
				SOUND_Play(&(comp_ctx.comp_sound_auto_turnoff));
				SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_auto_turnoff));
				comp_ctx.comp_state = COMP_STATE_AUTO_TURNOFF;
			}
			else {
				if ((SOUND_GetPositionMs(&(comp_ctx.comp_sound_auto_reg_min)) > (SOUND_GetLengthMs(&(comp_ctx.comp_sound_auto_reg_min)) - COMP_AUTO_OFF_MARGIN_MS)) ||
					(SOUND_GetPositionMs(&(comp_ctx.comp_sound_auto_reg_max)) > (SOUND_GetLengthMs(&(comp_ctx.comp_sound_auto_reg_max)) - COMP_AUTO_OFF_MARGIN_MS))) {
					// Automatically return to off state.
					comp_ctx.comp_sound_request = COMP_SOUND_REQUEST_OFF;
					comp_ctx.comp_state = COMP_STATE_OFF;
				}
			}
		}
		break;
	case COMP_STATE_AUTO_TURNOFF:
		// Auto to direct mode.
		if (comp_ctx.comp_sound_request == COMP_SOUND_REQUEST_DIRECT) {
			// Save all auto sounds.
			SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_auto_reg_min));
			SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_auto_reg_max));
			SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_auto_turnoff));
			// Start playing direct turn-on sound.
			SOUND_SetVolume(&(comp_ctx.comp_sound_direct_turnon), 1.0); // No fade-in effect required.
			SOUND_Play(&(comp_ctx.comp_sound_direct_turnon));
			SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_direct_turnon));
			comp_ctx.comp_state = COMP_STATE_DIRECT_TURNON;
		}
		else {
			// Perform turn-off fade-in and all other fade-out.
			comp_auto_turnoff_fade_end = SOUND_FadeIn(&(comp_ctx.comp_sound_auto_turnoff), COMP_FADE_DURATION_MS);
			comp_auto_reg_min_fade_end = SOUND_FadeOut(&(comp_ctx.comp_sound_auto_reg_min), COMP_FADE_DURATION_MS);
			comp_auto_reg_max_fade_end = SOUND_FadeOut(&(comp_ctx.comp_sound_auto_reg_max), COMP_FADE_DURATION_MS);
			// Change state when effect is complete.
			if ((comp_auto_turnoff_fade_end > 0) && (comp_auto_reg_min_fade_end > 0) && (comp_auto_reg_max_fade_end > 0)) {
				// Stop regulation min and max sounds.
				SOUND_Stop(&(comp_ctx.comp_sound_auto_reg_min));
				SOUND_Stop(&(comp_ctx.comp_sound_auto_reg_max));
				comp_ctx.comp_state = COMP_STATE_OFF;
			}
		}
		break;
	case COMP_STATE_DIRECT_TURNON:
		// Perform required fade-out effects.
		if (comp_ctx.comp_sound_request != COMP_SOUND_REQUEST_DIRECT) {
			// Save all other sounds volume.
			SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_direct_turnon));
			SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_direct_on1));
			SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_direct_on2));
			// Start playing turn-off sound.
			SOUND_SetVolume(&(comp_ctx.comp_sound_direct_turnoff), 0.0);
			SOUND_Play(&(comp_ctx.comp_sound_direct_turnoff));
			SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_direct_turnoff));
			comp_ctx.comp_state = COMP_STATE_DIRECT_TURNOFF;
		}
		else {
			if ((SOUND_GetPositionMs(&(comp_ctx.comp_sound_direct_turnon))) > ((SOUND_GetLengthMs(&(comp_ctx.comp_sound_direct_turnon))) - COMP_FADE_DURATION_MS - COMP_FADE_MARGIN_MS)) {
				// Save turn-on volume.
				SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_direct_turnon));
				// Start playing On1 sound.
				SOUND_SetVolume(&(comp_ctx.comp_sound_direct_on1), 0.0);
				SOUND_Play(&(comp_ctx.comp_sound_direct_on1));
				SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_direct_on1));
				// Stop playing turn-off sound (in case it was running).
				SOUND_Stop(&(comp_ctx.comp_sound_direct_turnoff));
				comp_ctx.comp_state = COMP_STATE_DIRECT_TURNON_TO_ON1;
			}
			else {
				// Perform all required fade-out effects.
				SOUND_FadeOut(&(comp_ctx.comp_sound_auto_reg_min), COMP_FADE_DURATION_MS);
				SOUND_FadeOut(&(comp_ctx.comp_sound_auto_reg_max), COMP_FADE_DURATION_MS);
				SOUND_FadeOut(&(comp_ctx.comp_sound_auto_turnoff), COMP_FADE_DURATION_MS);
			}
		}
		break;
	case COMP_STATE_DIRECT_TURNON_TO_ON1:
		if (comp_ctx.comp_sound_request != COMP_SOUND_REQUEST_DIRECT) {
			// Stop any auto sound.
			SOUND_Stop(&(comp_ctx.comp_sound_auto_reg_min));
			SOUND_Stop(&(comp_ctx.comp_sound_auto_reg_max));
			SOUND_Stop(&(comp_ctx.comp_sound_auto_turnoff));
			// Save all other sounds volume.
			SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_direct_turnon));
			SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_direct_on1));
			SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_direct_on2));
			// Start playing turn-off sound.
			SOUND_SetVolume(&(comp_ctx.comp_sound_direct_turnoff), 0.0);
			SOUND_Play(&(comp_ctx.comp_sound_direct_turnoff));
			SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_direct_turnoff));
			comp_ctx.comp_state = COMP_STATE_DIRECT_TURNOFF;
		}
		else {
			// Perform turn-on fade-out and On1 fade-in.
			comp_direct_on1_fade_end = SOUND_FadeIn(&(comp_ctx.comp_sound_direct_on1), COMP_FADE_DURATION_MS);
			comp_direct_turnon_fade_end = SOUND_FadeOut(&(comp_ctx.comp_sound_direct_turnon), COMP_FADE_DURATION_MS);
			// Change state when effect is complete.
			if ((comp_direct_on1_fade_end > 0) && (comp_direct_turnon_fade_end > 0)) {
				// Stop turn-on sound.
				SOUND_Stop(&(comp_ctx.comp_sound_direct_turnon));
				comp_ctx.comp_state = COMP_STATE_DIRECT_ON1;
			}
		}
		break;
	case COMP_STATE_DIRECT_ON1:
		if (comp_ctx.comp_sound_request != COMP_SOUND_REQUEST_DIRECT) {
			// Save all other sounds volume.
			SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_direct_turnon));
			SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_direct_on1));
			SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_direct_on2));
			// Start playing turn-off sound.
			SOUND_SetVolume(&(comp_ctx.comp_sound_direct_turnoff), 0.0);
			SOUND_Play(&(comp_ctx.comp_sound_direct_turnoff));
			SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_direct_turnoff));
			comp_ctx.comp_state = COMP_STATE_DIRECT_TURNOFF;
		}
		else {
			if ((SOUND_GetPositionMs(&(comp_ctx.comp_sound_direct_on1))) > ((SOUND_GetLengthMs(&(comp_ctx.comp_sound_direct_on1))) - COMP_FADE_DURATION_MS - COMP_FADE_MARGIN_MS)) {
				// Save On1 volume.
				SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_direct_on1));
				// Start playing On2 sound.
				SOUND_SetVolume(&(comp_ctx.comp_sound_direct_on2), 0.0);
				SOUND_Play(&(comp_ctx.comp_sound_direct_on2));
				SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_direct_on2));
				comp_ctx.comp_state = COMP_STATE_DIRECT_ON1_TO_ON2;
			}
		}
		break;
	case COMP_STATE_DIRECT_ON1_TO_ON2:
		if (comp_ctx.comp_sound_request != COMP_SOUND_REQUEST_DIRECT) {
			// Save all other sounds volume.
			SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_direct_turnon));
			SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_direct_on1));
			SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_direct_on2));
			// Start playing turn-off sound.
			SOUND_SetVolume(&(comp_ctx.comp_sound_direct_turnoff), 0.0);
			SOUND_Play(&(comp_ctx.comp_sound_direct_turnoff));
			SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_direct_turnoff));
			comp_ctx.comp_state = COMP_STATE_DIRECT_TURNOFF;
		}
		else {
			// Perform On1 fade-out and On2 fade-in.
			comp_direct_on2_fade_end = SOUND_FadeIn(&(comp_ctx.comp_sound_direct_on2), COMP_FADE_DURATION_MS);
			comp_direct_on1_fade_end = SOUND_FadeOut(&(comp_ctx.comp_sound_direct_on1), COMP_FADE_DURATION_MS);
			// Change state when effect is complete.
			if ((comp_direct_on2_fade_end > 0) && (comp_direct_on1_fade_end > 0)) {
				// Stop On1 sound.
				SOUND_Stop(&(comp_ctx.comp_sound_direct_on1));
				comp_ctx.comp_state = COMP_STATE_DIRECT_ON2;
			}
		}
		break;
	case COMP_STATE_DIRECT_ON2:
		if (comp_ctx.comp_sound_request != COMP_SOUND_REQUEST_DIRECT) {
			// Save all other sounds volume.
			SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_direct_turnon));
			SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_direct_on1));
			SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_direct_on2));
			// Start playing turn-off sound.
			SOUND_SetVolume(&(comp_ctx.comp_sound_direct_turnoff), 0.0);
			SOUND_Play(&(comp_ctx.comp_sound_direct_turnoff));
			SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_direct_turnoff));
			comp_ctx.comp_state = COMP_STATE_DIRECT_TURNOFF;
		}
		else {
			if ((SOUND_GetPositionMs(&(comp_ctx.comp_sound_direct_on2))) > ((SOUND_GetLengthMs(&(comp_ctx.comp_sound_direct_on2))) - COMP_FADE_DURATION_MS - COMP_FADE_MARGIN_MS)) {
				// Save On2 volume.
				SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_direct_on2));
				// Start playing On1 sound.
				SOUND_SetVolume(&(comp_ctx.comp_sound_direct_on1), 0.0);
				SOUND_Play(&(comp_ctx.comp_sound_direct_on1));
				SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_direct_on1));
				comp_ctx.comp_state = COMP_STATE_DIRECT_ON2_TO_ON1;
			}
		}
		break;
	case COMP_STATE_DIRECT_ON2_TO_ON1:
		if (comp_ctx.comp_sound_request != COMP_SOUND_REQUEST_DIRECT) {
			// Save all other sounds volume.
			SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_direct_turnon));
			SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_direct_on1));
			SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_direct_on2));
			// Start playing turn-off sound.
			SOUND_SetVolume(&(comp_ctx.comp_sound_direct_turnoff), 0.0);
			SOUND_Play(&(comp_ctx.comp_sound_direct_turnoff));
			SOUND_SaveFadeParameters(&(comp_ctx.comp_sound_direct_turnoff));
			comp_ctx.comp_state = COMP_STATE_DIRECT_TURNOFF;
		}
		else {
			// Perform On2 fade-out and On1 fade-in.
			comp_direct_on1_fade_end = SOUND_FadeIn(&(comp_ctx.comp_sound_direct_on1), COMP_FADE_DURATION_MS);
			comp_direct_on2_fade_end = SOUND_FadeOut(&(comp_ctx.comp_sound_direct_on2), COMP_FADE_DURATION_MS);
			// Change state when effect is complete.
			if ((comp_direct_on1_fade_end > 0) && (comp_direct_on2_fade_end > 0)) {
				// Stop On2 sound.
				SOUND_Stop(&(comp_ctx.comp_sound_direct_on2));
				comp_ctx.comp_state = COMP_STATE_DIRECT_ON1;
			}
		}
		break;
	case COMP_STATE_DIRECT_TURNOFF:
		// Perform turn-off fade-in and all other fade-out.
		comp_direct_turnoff_fade_end = SOUND_FadeIn(&(comp_ctx.comp_sound_direct_turnoff), COMP_FADE_DURATION_MS);
		comp_direct_turnon_fade_end = SOUND_FadeOut(&(comp_ctx.comp_sound_direct_turnon), COMP_FADE_DURATION_MS);
		comp_direct_on1_fade_end = SOUND_FadeOut(&(comp_ctx.comp_sound_direct_on1), COMP_FADE_DURATION_MS);
		comp_direct_on2_fade_end = SOUND_FadeOut(&(comp_ctx.comp_sound_direct_on2), COMP_FADE_DURATION_MS);
		// Change state when effect is complete.
		if ((comp_direct_turnoff_fade_end > 0) && (comp_direct_turnon_fade_end > 0) && (comp_direct_on1_fade_end > 0) && (comp_direct_on2_fade_end > 0)) {
			// Stop TurnOn, On1 and On2 sounds.
			SOUND_Stop(&(comp_ctx.comp_sound_direct_turnon));
			SOUND_Stop(&(comp_ctx.comp_sound_direct_on1));
			SOUND_Stop(&(comp_ctx.comp_sound_direct_on2));
			comp_ctx.comp_state = COMP_STATE_OFF;
		}
		break;
	default:
		// Unknown state.
		comp_ctx.comp_state = COMP_STATE_OFF;
		break;
	}
}
