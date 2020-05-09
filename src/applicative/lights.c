/*
 * lights.c
 *
 *  Created on: 9 may 2020
 *      Author: Ludo
 */

#include "lights.h"

#include "keyboard.h"
#include "openrails.h"
#include "stdio.h"

/*** LIGHTS local macros ***/

#define LIGHTS_LOG

/*** LIGHTS local structures ***/

typedef enum {
	LIGHTS_ZFG_STATUS_BIT_INDEX,
	LIGHTS_ZFD_STATUS_BIT_INDEX,
	LIGHTS_ZPR_STATUS_BIT_INDEX
} LIGHTS_StatusBitIndex;

typedef enum {
	LIGHTS_STATE_OFF,
	LIGHTS_STATE_ON
} LIGHTS_State;

typedef struct {
	unsigned char lights_status;
	LIGHTS_State lights_state;
} LIGHTS_Context;

/*** LIGHTS local global variables ***/

static LIGHTS_Context lights_ctx;

/*** LIGHTS functions ***/

/* INIT LIGHT MODULE.
 * @param:	None.
 * @return:	None.
 */
void LIGHTS_Init(void) {
	lights_ctx.lights_status = 0;
	lights_ctx.lights_state = LIGHTS_STATE_OFF;
}

/* TURN ZFG ON.
 * @param:	None.
 * @return:	None.
 */
void LIGHTS_ZfgOn(void) {
	lights_ctx.lights_status |= (0b1 << LIGHTS_ZFG_STATUS_BIT_INDEX);
#ifdef LIGHTS_LOG
	printf("LIGHTS *** ZFG on\n");
	fflush(stdout);
#endif
}

/* TURN ZFG OFF.
 * @param:	None.
 * @return:	None.
 */
void LIGHTS_ZfgOff(void) {
	lights_ctx.lights_status &= ~(0b1 << LIGHTS_ZFG_STATUS_BIT_INDEX);
#ifdef LIGHTS_LOG
	printf("LIGHTS *** ZFG off\n");
	fflush(stdout);
#endif
}

/* TURN ZFD ON.
 * @param:	None.
 * @return:	None.
 */
void LIGHTS_ZfdOn(void) {
	lights_ctx.lights_status |= (0b1 << LIGHTS_ZFD_STATUS_BIT_INDEX);
#ifdef LIGHTS_LOG
	printf("LIGHTS *** ZFD on\n");
	fflush(stdout);
#endif
}

/* TURN ZFD OFF.
 * @param:	None.
 * @return:	None.
 */
void LIGHTS_ZfdOff(void) {
	lights_ctx.lights_status &= ~(0b1 << LIGHTS_ZFD_STATUS_BIT_INDEX);
#ifdef LIGHTS_LOG
	printf("LIGHTS *** ZFD off\n");
	fflush(stdout);
#endif
}

/* TURN ZPR ON.
 * @param:	None.
 * @return:	None.
 */
void LIGHTS_ZprOn(void) {
	lights_ctx.lights_status |= (0b1 << LIGHTS_ZPR_STATUS_BIT_INDEX);
#ifdef LIGHTS_LOG
	printf("LIGHTS *** ZPR on\n");
	fflush(stdout);
#endif
}

/* TURN ZPR OFF.
 * @param:	None.
 * @return:	None.
 */
void LIGHTS_ZprOff(void) {
	lights_ctx.lights_status &= ~(0b1 << LIGHTS_ZPR_STATUS_BIT_INDEX);
#ifdef LIGHTS_LOG
	printf("LIGHTS *** ZPR off\n");
	fflush(stdout);
#endif
}

/* MAIN TASK OF LIGHTS MODULE.
 * @param:	None.
 * @return:	None.
 */
void LIGHTS_Task(void) {
	// Perform state machine.
	switch (lights_ctx.lights_state) {
	case LIGHTS_STATE_OFF:
		if (lights_ctx.lights_status != 0) {
			// Send keyboard control.
			KEYBOARD_Send(OPENRAILS_LIGHTS_ON, OPENRAILS_PRESS_DURATION_MS_DEFAULT);
			KEYBOARD_Send(OPENRAILS_LIGHTS_ON, OPENRAILS_PRESS_DURATION_MS_DEFAULT);
			lights_ctx.lights_state = LIGHTS_STATE_ON;
		}
		break;
	case LIGHTS_STATE_ON:
		if (lights_ctx.lights_status == 0) {
			// Send keyboard control.
			KEYBOARD_Send(OPENRAILS_LIGHTS_OFF, OPENRAILS_PRESS_DURATION_MS_DEFAULT);
			KEYBOARD_Send(OPENRAILS_LIGHTS_OFF, OPENRAILS_PRESS_DURATION_MS_DEFAULT);
			lights_ctx.lights_state = LIGHTS_STATE_OFF;
		}
		break;
	default:
		lights_ctx.lights_state = LIGHTS_STATE_OFF;
		break;
	}
}
