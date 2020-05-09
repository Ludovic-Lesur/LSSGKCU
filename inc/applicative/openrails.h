/*
 * openrails.h
 *
 *  Created on: 30 jul. 2017
 *      Author: Ludo
 */

#ifndef OPENRAILS_H
#define OPENRAILS_H

#include "keyboard.h"

/*** OPENRAILS shortcuts definition ***/

// Press duration to make the shortcuts work (in milliseconds).
#define OPENRAILS_PRESS_DURATION_MS_DEFAULT 	100
#define OPENRAILS_PRESS_DURATION_MS_TRAC 		300
// ZPT.
#define OPENRAILS_ZPT_FRONT_TOGGLE				&KEY_A
#define OPENRAILS_ZPT_BACK_TOGGLE				&KEY_B
// MPINV.
#define OPENRAILS_MPINV_FORWARD					&KEY_C
#define OPENRAILS_MPINV_BACKWARD				&KEY_D
// MP.
#define OPENRAILS_MP_TP							&KEY_E
#define OPENRAILS_MP_TM 						&KEY_F
#define OPENRAILS_MP_T0 						&KEY_G
// FPB.
#define OPENRAILS_FPB_APPLY 					&KEY_H
#define OPENRAILS_FPB_RELEASE 					&KEY_I
// FD.
#define OPENRAILS_FD_APPLY 						&KEY_J
#define OPENRAILS_FD_RELEASE					&KEY_K
// BPURG.
#define OPENRAILS_BPURG 						&KEY_L
// AUX.
#define OPENRAILS_BPEV 							&KEY_N
#define OPENRAILS_SABLAGE 						&KEY_O
// Lights.
#define OPENRAILS_LIGHTS_ON						&KEY_P
#define OPENRAILS_LIGHTS_OFF 					&KEY_Q
// Game.
#define OPENRAILS_MASK_CAB 						&KEY_W
#define OPENRAILS_CHANGE_CAB 					&KEY_X
#define OPENRAILS_LOG							&KEY_Y
#define OPENRAILS_NOP							&KEY_Z

#endif /* OPENRAILS_H */
