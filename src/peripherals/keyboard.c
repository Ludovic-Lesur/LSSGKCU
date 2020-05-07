/*
 * keyboard.c
 *
 *  Created on: 30 jul. 2017
 *      Author: Ludo
 */

#include "keyboard.h"

#include "stdio.h"
#include "windows.h"

/*** KEYBOARD macros ***/

#define KEYBOARD_LOG

/*** KEYBOARD extern variables initialisation ***/

const char KEY_A[KEY_ARRAY_SIZE] = {0x41, 0x9E};
const char KEY_B[KEY_ARRAY_SIZE] = {0x42, 0xB0};
const char KEY_C[KEY_ARRAY_SIZE] = {0x43, 0xAE};
const char KEY_D[KEY_ARRAY_SIZE] = {0x44, 0xA0};
const char KEY_E[KEY_ARRAY_SIZE] = {0x45, 0x92};
const char KEY_F[KEY_ARRAY_SIZE] = {0x46, 0xA1};
const char KEY_G[KEY_ARRAY_SIZE] = {0x47, 0xA2};
const char KEY_H[KEY_ARRAY_SIZE] = {0x48, 0xA3};
const char KEY_I[KEY_ARRAY_SIZE] = {0x49, 0x97};
const char KEY_J[KEY_ARRAY_SIZE] = {0x4A, 0xA4};
const char KEY_K[KEY_ARRAY_SIZE] = {0x4B, 0xA5};
const char KEY_L[KEY_ARRAY_SIZE] = {0x4C, 0xA6};
const char KEY_M[KEY_ARRAY_SIZE] = {0x4D, 0xB2};
const char KEY_N[KEY_ARRAY_SIZE] = {0x4E, 0xB1};
const char KEY_O[KEY_ARRAY_SIZE] = {0x4F, 0x98};
const char KEY_P[KEY_ARRAY_SIZE] = {0x50, 0x99};
const char KEY_Q[KEY_ARRAY_SIZE] = {0x51, 0x90};
const char KEY_R[KEY_ARRAY_SIZE] = {0x52, 0x93};
const char KEY_S[KEY_ARRAY_SIZE] = {0x53, 0x9F};
const char KEY_T[KEY_ARRAY_SIZE] = {0x54, 0x94};
const char KEY_U[KEY_ARRAY_SIZE] = {0x55, 0x96};
const char KEY_V[KEY_ARRAY_SIZE] = {0x56, 0xAF};
const char KEY_W[KEY_ARRAY_SIZE] = {0x57, 0x91};
const char KEY_X[KEY_ARRAY_SIZE] = {0x58, 0xAD};
const char KEY_Y[KEY_ARRAY_SIZE] = {0x59, 0x95};
const char KEY_Z[KEY_ARRAY_SIZE] = {0x5A, 0xAC};

/*** KEYBOARD functions ***/

/* SEND A KEYBOARD STROKE.
 * @param key:	Key to send.
 * @return: 	None.
 */
void KEYBOARD_Write(const char key[KEY_ARRAY_SIZE]) {
	keybd_event(key[KEY_CODE_INDEX], key[KEY_SCAN_INDEX], 0, 0);
#ifdef KEYBOARD_LOG
	printf("KEYBOARD *** Write key 0x%x\n", key[KEY_CODE_INDEX]);
	fflush(stdout);
#endif
}

/* PRESS A KEYBOARD KEY.
 * @param key:	Key to press.
 * @return:		None.
 */
void KEYBOARD_Press(const char key[KEY_ARRAY_SIZE]) {
	keybd_event(key[KEY_CODE_INDEX], key[KEY_SCAN_INDEX], 0, WM_KEYDOWN);
#ifdef KEYBOARD_LOG
	printf("KEYBOARD *** Press key 0x%x\n", key[KEY_CODE_INDEX]);
	fflush(stdout);
#endif
}

/* RELEASE A KEYBOARD KEY.
 * @param key:	Key to release.
 * @return:		None.
 */
void Keyboard_Release(const char key[KEY_ARRAY_SIZE]) {
	keybd_event(key[KEY_CODE_INDEX], key[KEY_SCAN_INDEX], 0, WM_KEYUP);
#ifdef KEYBOARD_LOG
	printf("KEYBOARD *** Release key 0x%x\n", key[KEY_CODE_INDEX]);
	fflush(stdout);
#endif
}
