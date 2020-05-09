/*
 * mpinv.c
 *
 *  Created on: 9 may 2020
 *      Author: Ludo
 */

#include "mpinv.h"

#include "keyboard.h"
#include "mixer.h"
#include "openrails.h"
#include "sound.h"
#include "stdio.h"

/*** MPINV local macros ***/

#define MPINV_LOG

/*** MPINV local structures ***/

typedef enum {
	MPINV_STATE_NEUTRAL,
	MPINV_STATE_FORWARD,
	MPINV_STATE_BACKWARD
} MPINV_State;

typedef struct {
	SOUND_Context mpinv_forward_backward_sound;
	SOUND_Context mpinv_neutral_sound;
	MPINV_State mpinv_state;
} MPINV_Context;

/*** MPINV local global variables ***/

static MPINV_Context mpinv_ctx;

/*** MPINV functions ***/

/* INIT MPINV MODULE.
 * @param:	None.
 * @return:	None.
 */
void MPINV_Init(void) {
	// Init sounds.
	SOUND_Init(&(mpinv_ctx.mpinv_forward_backward_sound), "mpinv_forward_backward.wav", MPINV_AUDIO_GAIN);
	SOUND_SetVolume(&(mpinv_ctx.mpinv_forward_backward_sound), 1.0); // No fade effect required.
	SOUND_Init(&(mpinv_ctx.mpinv_neutral_sound), "mpinv_neutral.wav", MPINV_AUDIO_GAIN);
	SOUND_SetVolume(&(mpinv_ctx.mpinv_neutral_sound), 1.0); // No fade effect required.
}

/* MOVE INVERSOR TO FORWARD.
 * @param:	None.
 * @return:	None.
 */
void MPINV_Forward(void) {
	// Play sound.
	SOUND_Play(&(mpinv_ctx.mpinv_forward_backward_sound));
	// Send OpenRails shortcut.
	KEYBOARD_Send(OPENRAILS_MPINV_FORWARD, OPENRAILS_PRESS_DURATION_MS_DEFAULT);
	// Update state.
	mpinv_ctx.mpinv_state = MPINV_STATE_FORWARD;
#ifdef MPINV_LOG
	printf("MPINV *** Forward.\n");
	fflush(stdout);
#endif
}

/* MOVE INVERSOR TO NEUTRAL.
 * @param:	None.
 * @return:	None.
 */
void MPINV_Neutral(void) {
	// Play sound.
	SOUND_Play(&(mpinv_ctx.mpinv_neutral_sound));
	// Send accurate OpenRails shortcut.
	switch (mpinv_ctx.mpinv_state) {
	case MPINV_STATE_FORWARD:
		// Previous state was forward.
		KEYBOARD_Send(OPENRAILS_MPINV_BACKWARD, OPENRAILS_PRESS_DURATION_MS_DEFAULT);
		break;
	case MPINV_STATE_BACKWARD:
		// Previous state was backward.
		KEYBOARD_Send(OPENRAILS_MPINV_FORWARD, OPENRAILS_PRESS_DURATION_MS_DEFAULT);
		break;
	default:
		break;
	}
	// UPdate state.
	mpinv_ctx.mpinv_state = MPINV_STATE_NEUTRAL;
#ifdef MPINV_LOG
	printf("MPINV *** Neutral.\n");
	fflush(stdout);
#endif
}

/* MOVE INVERSOR TO BACKWARD.
 * @param:	None.
 * @return:	None.
 */
void MPINV_Backward(void) {
	// Play sound.
	SOUND_Play(&(mpinv_ctx.mpinv_forward_backward_sound));
	// Send OpenRails shortcut.
	KEYBOARD_Send(OPENRAILS_MPINV_BACKWARD, OPENRAILS_PRESS_DURATION_MS_DEFAULT);
	// Update state.
	mpinv_ctx.mpinv_state = MPINV_STATE_BACKWARD;
#ifdef MPINV_LOG
	printf("MPINV *** Backward.\n");
	fflush(stdout);
#endif
}

