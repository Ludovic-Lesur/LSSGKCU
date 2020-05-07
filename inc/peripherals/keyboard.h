/*
 * keyboard.h
 *
 *  Created on: 30 jul. 2017
 *      Author: Ludo
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

/*** KEYBOARD macros ***/

#define KEY_ARRAY_SIZE	2
#define KEY_CODE_INDEX 	0
#define KEY_SCAN_INDEX 	1

/*** KEYBAORD extern variables ***/

extern const char KEY_A[KEY_ARRAY_SIZE];
extern const char KEY_B[KEY_ARRAY_SIZE];
extern const char KEY_C[KEY_ARRAY_SIZE];
extern const char KEY_D[KEY_ARRAY_SIZE];
extern const char KEY_E[KEY_ARRAY_SIZE];
extern const char KEY_F[KEY_ARRAY_SIZE];
extern const char KEY_G[KEY_ARRAY_SIZE];
extern const char KEY_H[KEY_ARRAY_SIZE];
extern const char KEY_I[KEY_ARRAY_SIZE];
extern const char KEY_J[KEY_ARRAY_SIZE];
extern const char KEY_K[KEY_ARRAY_SIZE];
extern const char KEY_L[KEY_ARRAY_SIZE];
extern const char KEY_M[KEY_ARRAY_SIZE];
extern const char KEY_N[KEY_ARRAY_SIZE];
extern const char KEY_O[KEY_ARRAY_SIZE];
extern const char KEY_P[KEY_ARRAY_SIZE];
extern const char KEY_Q[KEY_ARRAY_SIZE];
extern const char KEY_R[KEY_ARRAY_SIZE];
extern const char KEY_S[KEY_ARRAY_SIZE];
extern const char KEY_T[KEY_ARRAY_SIZE];
extern const char KEY_U[KEY_ARRAY_SIZE];
extern const char KEY_V[KEY_ARRAY_SIZE];
extern const char KEY_W[KEY_ARRAY_SIZE];
extern const char KEY_X[KEY_ARRAY_SIZE];
extern const char KEY_Y[KEY_ARRAY_SIZE];
extern const char KEY_Z[KEY_ARRAY_SIZE];

/*** KEYBOARD functions ***/

void KEYBOARD_Write(const char key[KEY_ARRAY_SIZE]);
void KEYBOARD_Press(const char key[KEY_ARRAY_SIZE]);
void KEYBOARD_Release(const char key[KEY_ARRAY_SIZE]);

#endif /* KEYBOARD_H */
