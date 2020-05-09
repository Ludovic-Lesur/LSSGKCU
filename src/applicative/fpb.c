/*
 * fpb.c
 *
 *  Created on: 9 mai 2020
 *      Author: Ludovic
 */

#include "fpb.h"

#include "keyboard.h"
#include "mixer.h"
#include "openrails.h"
#include "sound.h"
#include "stdio.h"

/*** FPB local macros ***/

#define FPB_LOG

/*** FPB structures ***/

typedef struct {
	SOUND_Context fpb_on_sound;
	SOUND_Context fpb_apply_release_sound;
	SOUND_Context fpb_neutral_sound;
} FPB_Context;

/*** FPB local global variables ***/

static FPB_Context fpb_ctx;

/*** FPB functions ***/

/* INIT FPB MODULE.
 * @param:	None.
 * @return:	None.
 */
void FPB_Init(void) {
	// Init sounds.
	SOUND_Init(&(fpb_ctx.fpb_apply_release_sound), "fpb_apply_release.wav", FPB_AUDIO_GAIN);
	SOUND_SetVolume(&(fpb_ctx.fpb_apply_release_sound), 1.0); // No fade effect required.
	SOUND_Init(&(fpb_ctx.fpb_neutral_sound), "fpb_neutral.wav", FPB_AUDIO_GAIN);
	SOUND_SetVolume(&(fpb_ctx.fpb_neutral_sound), 1.0); // No fade effect required.
}

/* TURN FPB MODULE ON.
 * @param:	None.
 * @return:	None.
 */
void FPB_On(void) {

}

/* TURN FPB MODULE OFF.
 * @param:	None.
 * @return:	None.
 */
void FPB_Off(void) {

}

/* APPLY FPB.
 * @param:	None.
 * @return:	None.
 */
void FPB_Apply(void) {
	// Play sound.
	SOUND_Play(&(fpb_ctx.fpb_apply_release_sound));
	// Send OpenRails shortcut.
	KEYBOARD_Send(OPENRAILS_FPB_APPLY, OPENRAILS_PRESS_DURATION_MS_DEFAULT);
#ifdef FPB_LOG
	printf("FPB *** Apply.\n");
	fflush(stdout);
#endif
}

/* SET FPB TO NEUTRAL.
 * @param:	None.
 * @return:	None.
 */
void FPB_Neutral(void) {
	// Play sound.
	SOUND_Play(&(fpb_ctx.fpb_neutral_sound));
	SOUND_Stop(&(fpb_ctx.fpb_apply_release_sound));
#ifdef FPB_LOG
	printf("FPB *** Neutral.\n");
	fflush(stdout);
#endif
}

/* RELEASE FPB.
 * @param:	None.
 * @return:	None.
 */
void FPB_Release(void) {
	// Play sound.
	SOUND_Play(&(fpb_ctx.fpb_apply_release_sound));
	// Send OpenRails shortcut.
	KEYBOARD_Send(OPENRAILS_FPB_RELEASE, OPENRAILS_PRESS_DURATION_MS_DEFAULT);
#ifdef FPB_LOG
	printf("FPB *** Release.\n");
	fflush(stdout);
#endif
}
