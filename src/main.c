/*
 * main.c
 *
 *  Created on: 25 mar. 2020
 *      Author: Ludo
 */

#include "keyboard.h"
#include "serial.h"
#include "sound.h"
#include "stdio.h"

/* MAIN FUNCTION.
 * @param:	None.
 * @return:	None.
 */
int main (void) {
	// Start print.
	printf("**********************************************************************\n");
	printf("*** Locomotive Simulator Sound Game and KVB Control Unit (LSSGKCU) ***\n");
	printf("**********************************************************************\n\n");
	fflush(stdout);
	// Init all modules.
	SOUND_FmodSystemInit();
	HANDLE lsmcu;
	SERIAL_Open(&lsmcu, "COM3", 9600);
	// Sound test.
	SOUND_Context zba;
	SOUND_Create(&zba, "C:/Users/Ludovic/Documents/Eclipse/LSSGKCU/wav/zbaTurnOn.wav", 1.0);
	SOUND_Play(&zba);
	// Keyboard test.
	KEYBOARD_Write(KEY_A);
	// Serial port test.
	while (1) {
		SERIAL_Read(&lsmcu);
	}
	printf("End of program");

	return 0;
}
