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

// No-operation.
static KEYBOARD_Key* OPENRAILS_NOP = &KEY_Z;

// Pantographes.
static KEYBOARD_Key* OPENRAILS_ZPT_FRONT_TOGGLE = &KEY_A;
static KEYBOARD_Key* OPENRAILS_ZPT_BACK_TOGGLE = &KEY_B;

// Inverseur.
static KEYBOARD_Key* OPENRAILS_INV_AV = &KEY_C;
static KEYBOARD_Key* OPENRAILS_INV_AR = &KEY_D;

// Manipulateur de traction.
static KEYBOARD_Key* OPENRAILS_MP_TP = &KEY_E;
static KEYBOARD_Key* OPENRAILS_MP_TM = &KEY_F;
static KEYBOARD_Key* OPENRAILS_MP_T0 = &KEY_G;


// Freinage.
static KEYBOARD_Key* OPENRAILS_FPB_P = &KEY_H;
static KEYBOARD_Key* OPENRAILS_FPB_M = &KEY_I;
static KEYBOARD_Key* OPENRAILS_FD_P = &KEY_J;
static KEYBOARD_Key* OPENRAILS_FD_M = &KEY_K;
static KEYBOARD_Key* OPENRAILS_BPURG = &KEY_L;

// Auxiliaires.
static KEYBOARD_Key* OPENRAILS_BPEV = &KEY_N;
static KEYBOARD_Key* OPENRAILS_SABLAGE = &KEY_O;

// Fanaux et projecteurs.
static KEYBOARD_Key* OPENRAILS_LIGHTS_ON = &KEY_P;
static KEYBOARD_Key* OPENRAILS_LIGHTS_OFF = &KEY_Q;

// Divers.
static KEYBOARD_Key* OPENRAILS_MASK_CAB = &KEY_W;
static KEYBOARD_Key* OPENRAILS_CHANGE_CAB = &KEY_X;

// Log.
static KEYBOARD_Key* OPENRAILS_LOG = &KEY_Y;

#endif /* OPENRAILS_H */
