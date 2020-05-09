/*
 * zpt.c
 *
 *  Created on: 26 mar. 2020
 *      Author: Ludo
 */

#include "zpt.h"

#include "keyboard.h"
#include "mixer.h"
#include "openrails.h"
#include "sound.h"
#include "stdio.h"

/*** ZPT local macros ***/

#define ZPT_LOG

/*** ZPT local structures ***/

typedef struct {
	SOUND_Context zpt_back_up_sound;
	SOUND_Context zpt_back_down_sound;
	SOUND_Context zpt_front_up_sound;
	SOUND_Context zpt_front_down_sound;
	unsigned char zpt_back_raised;
	unsigned char zpt_front_raised;
} ZPT_Context;

/*** ZPT local global variables ***/

static ZPT_Context zpt_ctx;

/*** ZPT functions ***/

/* INIT ZPT SOUND MODULE.
 * @param:	None.
 * @return:	None.
 */
void ZPT_Init(void) {
	// Init sounds.
	SOUND_Init(&(zpt_ctx.zpt_back_up_sound), "zpt_up.wav", ZPT_BACK_AUDIO_GAIN);
	SOUND_SetVolume(&(zpt_ctx.zpt_back_up_sound), 1.0); // No fade effect required.
	SOUND_Init(&(zpt_ctx.zpt_back_down_sound), "zpt_down.wav", ZPT_BACK_AUDIO_GAIN);
	SOUND_SetVolume(&(zpt_ctx.zpt_back_down_sound), 1.0); // No fade effect required.
	SOUND_Init(&(zpt_ctx.zpt_front_up_sound), "zpt_up.wav", ZPT_FRONT_AUDIO_GAIN);
	SOUND_SetVolume(&(zpt_ctx.zpt_front_up_sound), 1.0); // No fade effect required.
	SOUND_Init(&(zpt_ctx.zpt_front_down_sound), "zpt_down.wav", ZPT_FRONT_AUDIO_GAIN);
	SOUND_SetVolume(&(zpt_ctx.zpt_front_down_sound), 1.0); // No fade effect required.
	// Init context.
	zpt_ctx.zpt_back_raised = 0;
	zpt_ctx.zpt_front_raised = 0;
}

/* RAISE BACK PANTOGRAPH.
 * @param:	None.
 * @eturn:	None.
 */
void ZPT_BackUp(void) {
	// Play sound.
	SOUND_Play(&(zpt_ctx.zpt_back_up_sound));
	// Send OpenRails shortcut if state changed.
	if (zpt_ctx.zpt_back_raised == 0) {
		KEYBOARD_Send(OPENRAILS_ZPT_BACK_TOGGLE, OPENRAILS_PRESS_DURATION_MS_DEFAULT);
	}
	zpt_ctx.zpt_back_raised = 1;
#ifdef ZPT_LOG
	printf("ZPT *** Back up.\n");
	fflush(stdout);
#endif
}

/* LOWER BACK PANTOGRAPH.
 * @param:	None.
 * @eturn:	None.
 */
void ZPT_BackDown(void) {
	// Play sound.
	SOUND_Play(&(zpt_ctx.zpt_back_down_sound));
	SOUND_Stop(&(zpt_ctx.zpt_back_up_sound));
	// Send OpenRails shortcut if state changed.
	if (zpt_ctx.zpt_back_raised != 0) {
		KEYBOARD_Send(OPENRAILS_ZPT_BACK_TOGGLE, OPENRAILS_PRESS_DURATION_MS_DEFAULT);
	}
	zpt_ctx.zpt_back_raised = 0;
#ifdef ZPT_LOG
	printf("ZPT *** Back down.\n");
	fflush(stdout);
#endif
}

/* RAISE FRONT PANTOGRAPH.
 * @param:	None.
 * @eturn:	None.
 */
void ZPT_FrontUp(void) {
	// Play sound.
	SOUND_Play(&(zpt_ctx.zpt_front_up_sound));
	// Send OpenRails shortcut if state changed.
	if (zpt_ctx.zpt_front_raised == 0) {
		KEYBOARD_Send(OPENRAILS_ZPT_FRONT_TOGGLE, OPENRAILS_PRESS_DURATION_MS_DEFAULT);
	}
	zpt_ctx.zpt_front_raised = 1;
#ifdef ZPT_LOG
	printf("ZPT *** Front up.\n");
	fflush(stdout);
#endif
}

/* LOWER FRONT PANTOGRAPH.
 * @param:	None.
 * @eturn:	None.
 */
void ZPT_FrontDown(void) {
	// Play sound.
	SOUND_Play(&(zpt_ctx.zpt_front_down_sound));
	SOUND_Stop(&(zpt_ctx.zpt_front_up_sound));
	// Send OpenRails shortcut if state changed.
	if (zpt_ctx.zpt_front_raised != 0) {
		KEYBOARD_Send(OPENRAILS_ZPT_FRONT_TOGGLE, OPENRAILS_PRESS_DURATION_MS_DEFAULT);
	}
	zpt_ctx.zpt_front_raised = 0;
#ifdef ZPT_LOG
	printf("ZPT *** Front down.\n");
	fflush(stdout);
#endif
}
