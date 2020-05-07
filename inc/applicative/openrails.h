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
#define OPENRAILS_PRESS_DURATION 		100

// No-operation.
#define OPENRAILS_NOP 					KEY_Z

// Pantographes.
#define OPENRAILS_ZPT_AR_TOGGLE 		KEY_A
#define OPENRAILS_ZPT_AV_TOGGLE 		KEY_B

// Inverseur.
#define OPENRAILS_INV_AV 				KEY_C
#define OPENRAILS_INV_AR 				KEY_D

// Manipulateur de traction.
#define OPENRAILS_MP_TP 				KEY_E
#define OPENRAILS_MP_TM 				KEY_F
#define OPENRAILS_MP_T0					KEY_G
#define OPENRAILS_PRESS_DURATION_TRAC 	300 // Durée d'appui des raccourcis clavier spécifique à la traction.

// Freinage.
#define OPENRAILS_FPB_P 				KEY_H
#define OPENRAILS_FPB_M 				KEY_I
#define OPENRAILS_FD_P 					KEY_J
#define OPENRAILS_FD_M 					KEY_K
#define OPENRAILS_BPURG 				KEY_L

// Auxiliaires.
#define OPENRAILS_BPEV 					KEY_N
#define OPENRAILS_SABLAGE 				KEY_O

// Fanaux et projecteurs.
#define OPENRAILS_ECLAIRAGE_ON 			KEY_P
#define OPENRAILS_ECLAIRAGE_OFF 		KEY_Q
#define OPENRAILS_PROJECTEURS 			K_NOP

// Divers.
#define OPENRAILS_MASK_CAB 				KEY_W
#define OPENRAILS_CHANGE_CAB 			KEY_X

// Log.
#define OPENRAILS_LOG 					KEY_Y

#endif /* OPENRAILS_H */
